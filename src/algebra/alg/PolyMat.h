//
// Created by thallock on 10/21/21.
//

#ifndef IDEA_POLY_MAT_H
#define IDEA_POLY_MAT_H

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseCore>

#include <ranges>
#include <vector>

#include "algebra/PolyDict.h"

#include "algebra/alg/dense_jordan.h"
#include "algebra/alg/sparse_jordan.h"


namespace algebra::poly {

	class PolyMatBase {
	public:
		int nrows;
		int ncols;
		std::vector<IndexPtr> terms;
		PolyPtr empty;

		PolyMatBase(std::set<IndexPtr, MonomialOrder>& terms, std::vector<PolyPtr>& polys)
			: nrows{(int) polys.size()}
			, ncols{(int) terms.size()}
			, terms{terms.rbegin(), terms.rend()}
			, empty{polys[0]->empty()}
		{}

		virtual ~PolyMatBase() = default;

		virtual void reduce() = 0;
	};

	/*[[nodiscard]] */inline
	size_t count_terms(std::vector<PolyPtr>& polys) {
		auto sum = 0;
		for (const auto& poly: polys) {
			sum += poly->terms.size();
		}
		return sum;
	}

	class PolySparseMat : public PolyMatBase {
	public:
		Eigen::SparseMatrix<double, Eigen::RowMajor> M;

		PolySparseMat(std::set<IndexPtr, MonomialOrder>& t, std::vector<PolyPtr>& polys)
			: PolyMatBase{t, polys}
			, M{(Eigen::Index) nrows, (Eigen::Index) ncols}
		{
			std::vector<Eigen::Triplet<double>> triples;
			triples.reserve(count_terms(polys));

			for (int r = 0; r < (int) polys.size(); r++) {
				const auto& poly = polys[r];
				for (int c = 0; c < terms.size(); c++) {
					const auto it = poly->terms.find(terms[c]);
					if (it != poly->terms.end()) {
						triples.emplace_back(r, c, it->second);
					}
				}
			}
			M.setFromTriplets(triples.begin(), triples.end());
		}

		[[nodiscard]] inline
		PolyPtr get_poly(int index) const {
			PolyPtr ret = empty->empty();
			for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(M, index); it; ++it) {
				if (it.row() != index) {
					throw std::runtime_error{"Don't know how to use the matrix..."};
				}
				double c = it.value();
				if (std::abs(c) < POLY_TOL) {
					continue;
				}
				ret->terms[terms[it.col()]] = c;
			}
			return ret;
		}

		void reduce() override {
			sparse_jordan(M);
		}
	};


	class PolyDenseMat : public PolyMatBase {
	public:
		Eigen::MatrixXd M;

		PolyDenseMat(std::set<IndexPtr, MonomialOrder>& t, std::vector<PolyPtr>& polys)
			: PolyMatBase{t, polys}
			, M{(Eigen::Index) nrows, (Eigen::Index) ncols}
		{
			int nz_count = 0;
			for (int r = 0; r < (int) polys.size(); r++) {
				const auto& poly = polys[r];
				for (int c = 0; c < (int) terms.size(); c++) {
					const auto it = poly->terms.find(terms[c]);
					if (it == poly->terms.end()) {
						M(r, c) = 0;
					} else {
						M(r, c) = it->second;
						nz_count++;
					}
				}
			}

			{
				std::string filename("num_nz.txt");
				std::ofstream file_out{filename, std::ios_base::app};
				file_out << "PERCENTAGE OF NONZERO: " << nz_count / (double) (nrows * ncols)
						  << " of " << (nrows * ncols) << "\n";
			}
		}

		[[nodiscard]] inline
		PolyPtr get_poly(int index) const {
			PolyPtr ret = empty->empty();
			for (int col = 0; col < terms.size(); col++) {
				double c = M(index, col);
				if (std::abs(c) < POLY_TOL) {
					continue;
				}
				ret->terms[terms[col]] = c; // .emplace(*tit, c);
			}
			return ret/*.to_monic()*/;
		}

		void reduce() override {
			dense_jordan(M);
		}
	};
}


#endif //IDEA_POLY_MAT_H
