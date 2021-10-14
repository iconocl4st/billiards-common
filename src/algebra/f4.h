//
// Created by thallock on 10/13/21.
//

#ifndef IDEA_F4_H
#define IDEA_F4_H

#include <list>
#include <set>
#include <map>
#include <climits>

#include <Eigen/Dense>

#include "GmpPoly.h"

namespace algebra::poly {

	class Pair {
	public:
		int idx1;
		int idx2;

		Pair(int idx1, int idx2) : idx1{idx1}, idx2{idx2} {}
		~Pair() = default;

		[[nodiscard]] inline
		bool operator<(const Pair& other) const {
			if (idx1 < other.idx1) {
				return true;
			}
			if (idx1 > other.idx1) {
				return false;
			}
			if (idx2 < other.idx2) {
				return true;
			}
			return false;
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const Pair& p) {
			return os << "{i:" << p.idx1 << ",j:" << p.idx2 << "}";
		}
	};

	void collect_terms(const PolyDict& poly, std::set<MultiIndex>& terms) {
		for (const auto& term : poly.terms) {
			terms.insert(term.first);
		}
	}
	void collect_terms(std::vector<PolyDict>& polys, std::set<MultiIndex>& terms) {
		for (const auto& poly : polys) {
			collect_terms(poly, terms);
		}
	}

	class F4State {
	public:
		std::vector<PolyDict>& polys;
		std::map<Pair, int> degrees;
		std::set<Pair> queue;

		explicit F4State(std::vector<PolyDict>& polynomials, std::vector<PolyDict>& basis)
			: polys{basis}
			, degrees{}
			, queue{}
		{}

		void add_polynomial(const PolyDict& poly) {
			int poly_index = (int) polys.size();
			for (int i = 0; i < poly_index; i++) {
				queue.emplace(i, poly_index);
			}

			const auto& leading = poly.leading();
			for (int i = 0; i < (int) polys.size(); i++) {
				std::cout << "Computing deg of pair " << i << "," << poly_index << std::endl;
				std::cout << "\tleading 1 = " << leading << std::endl;
				std::cout << "\tleading 2 = " << polys[i].leading() << std::endl;
				std::cout << "\tlcm = " <<  leading.lcm(polys[i].leading()) << std::endl;
				std::cout << "\tdegree = " << leading.lcm(polys[i].leading()).degree() << std::endl;
				degrees[Pair{i, poly_index}] = leading.lcm(polys[i].leading()).degree();
			}
			polys.push_back(poly);
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const F4State& state) {
			std::cout << "Current basis:" << std::endl;
			for (const auto& poly : state.polys) {
				std::cout << "\t" << poly << std::endl;
			}
			std::cout << "Degree of pairs:" << std::endl;
			for (const auto& deg : state.degrees) {
				std::cout << "\t" << deg.first << " -> " << deg.second << std::endl;
			}
			std::cout << "Queue:" << std::endl;
			for (const auto& p : state.queue) {
				std::cout << "\t" << p << std::endl;
			}
			return os;
		}
	};

	inline
	void select_subset(F4State& state, std::list<Pair>& B) {
		int m = INT_MAX;
		for (const auto& pair : state.queue) {
			auto degree = state.degrees.find(pair);
			if (degree == state.degrees.end()) {
				throw std::runtime_error{"Failed to compute the degree of a pair"};
			}
			if (degree->second < m) {
				B.clear();
				m = degree->second;
			}
			if (degree->second == m) {
				B.push_back(degree->first);
			}
		}
		for (const auto& pair : B) {
			auto it = state.queue.find(pair);
			if (it == state.queue.end()) {
				throw std::runtime_error{"We just put it here..."};
			}
			state.queue.erase(it);
		}
	}

	inline
	void construct_sygyzys(F4State& state, std::list<Pair>& B, std::vector<PolyDict>& sygyzys) {
		for (const auto& pair : B) {
			const auto& fi = state.polys[pair.idx1];
			const auto& fj = state.polys[pair.idx2];
			sygyzys.push_back(fi * (fi.leading().lcm(fj.leading()) / fi.leading()));
			sygyzys.push_back(fj * (fj.leading().lcm(fi.leading()) / fj.leading()));
		}
	}

	inline
	void construct_H(F4State& state, std::vector<PolyDict>& sygyzys, std::set<MultiIndex>& terms) {
		collect_terms(sygyzys, terms);
		std::set<MultiIndex> done;
		for (const auto& poly : sygyzys) {
			done.insert(poly.leading());
		}
		while (done.size() < terms.size()) {
			std::cout << "constructing h: done = ";
			for (const auto& m : done) {
				std::cout << m << " ";
			}
			std::cout << std::endl;
			std::cout << "constructing h: terms = ";
			for (const auto& m : terms) {
				std::cout << m << " ";
			}
			std::cout << std::endl;

			MultiIndex maxi{sygyzys[0].ideal};
			for (const auto& m : terms) {
				const auto& it = done.find(m);
				if (it != done.end()) {
					continue;
				}
				if (m > maxi) {
					maxi = m;
				}
			}
			done.insert(maxi);
			std::cout << "constructing h: found maximum degree: " << maxi << std::endl;

			for (const PolyDict& fl : state.polys) {
				const auto& leading = fl.leading();
				if (!leading.divides(maxi)) {
					continue;
				}
				const PolyDict h = fl * (maxi / leading);
				sygyzys.push_back(h);
				collect_terms(h, terms);

				std::cout << "constructing h: found polynomial f_l = " << fl << std::endl;
				std::cout << "constructing h: Computed h = " << h << std::endl;
				break;
			}
		}
	}

	inline
	void write_to_matrix(std::set<MultiIndex>& terms, std::vector<PolyDict>& sygyzys, Eigen::MatrixXd& M) {
		for (int r = 0; r < (int) sygyzys.size(); r++) {
			int c = 0;
			for (auto tit = terms.rbegin(); tit != terms.rend(); ++tit) {
				const auto it = sygyzys[r].terms.find(*tit);
				if (it == sygyzys[r].terms.end()) {
					M(r, c++) = 0;
				} else {
					M(r, c++) = it->second;
				}
			}
		}
	}

//	inline
//	bool in_nplus(F4State& state, const PolyDict& poly, std::set<MultiIndex>& terms, Eigen::MatrixXd& M, int index) {
//		if (poly.is_zero()) {
//			return false;
//		}
//		return true;
//	}


	int get_pivot_row(Eigen::MatrixXd& M, int r, int c) {
		int max_r = r;
		for (int i = r + 1; i < M.rows(); i++) {
			if (std::abs(M(i, c)) > std::abs(M(max_r, c))) {
				max_r = i;
			}
		}
		return max_r;
	}

	void pivot(Eigen::MatrixXd& M, std::vector<int>& pivots, int r, int max_r) {
		if (max_r == r) {
			return;
		}
		int tmp = pivots[r];
		pivots[r] = pivots[max_r];
		pivots[max_r] = tmp;

		M.row(r).swap(M.row(max_r));
	}

	void eliminate(Eigen::MatrixXd& M, int r, int c) {
		M.row(r) /= M(r, c);
		for (int i = 0; i < M.rows(); i++) {
			if (i == r) {
				continue;
			}
			M.row(i) -= M(i, c) * M.row(r);
		}
	}

	void reduce(Eigen::MatrixXd& M) {
		// Use eigen's permutation matrix...
		std::vector<int> pivots;
		for (int i = 0; i < M.rows(); i++) {
			pivots.push_back(i);
		}
		// There really ought to be an efficient method for this...
		int c = 0;
		int r = 0;
		while (r < M.rows() && c < M.cols()) {
			int max_r = get_pivot_row(M, r, c);
			for (int i = r + 1; i < M.rows(); i++) {
				if (std::abs(M(i, c)) > std::abs(M(i, c))) {
					max_r = i;
				}
			}
			pivot(M, pivots, r, max_r);

			if (std::abs(M(r, c)) < YET_ANOTHER_TOL) {
				++c;
				continue;
			}

			eliminate(M, r, c);
			++r; ++c;
		}
	}

	[[nodiscard]] inline
	PolyDict get_poly(std::set<MultiIndex>& terms, Eigen::MatrixXd& M, int index) {
		int col = 0;
		PolyDict p{terms.begin()->ideal};
		for (auto tit = terms.rbegin(); tit != terms.rend(); ++tit) {
			double c = M(index, col++);
			if (std::abs(c) < YET_ANOTHER_TOL) {
				continue;
			}
			p.terms.emplace(*tit, c);
		}
		return p/*.to_monic()*/;
	}

	bool leading_is_generated_by(const MultiIndex& leading, std::vector<MultiIndex>& indices, int index) {
		if (leading.is_constant()) {
			return true;
		}
		if (index >= (int) indices.size()) {
			return false;
		}
		MultiIndex r = leading;
		while (true) {
			if (leading_is_generated_by(r, indices, index + 1)) {
				return true;
			}
			if (!indices[index].divides(r)) {
				return false;
			}
			r = r / indices[index];
		}
	}

	bool leading_is_generated_by(const PolyDict& poly, std::vector<PolyDict>& sygyzys) {
		std::set<MultiIndex> lms; // Is this already calculated?
		for (const auto& p : sygyzys) {
			lms.insert(p.leading());
		}
		std::cout << "leading(H):" << std::endl;
		for (const auto& p : lms) {
			std::cout << "\t" << p << std::endl;
		}
		std::vector<MultiIndex> lmv{lms.begin(), lms.end()};

		return leading_is_generated_by(poly.leading(), lmv, 0);
	}

	void f4(std::vector<PolyDict> polys, std::vector<PolyDict>& basis) {
		F4State state{polys, basis};
		for (const PolyDict& poly : polys) {
			state.add_polynomial(poly.to_monic());
		}

		int count = 0;
		while (!state.queue.empty() && count++ < 3) {
			std::cout << "======================================" << std::endl;
			std::cout << "State:" << std::endl;
			std::cout << state << std::endl;

			std::list<Pair> B;
			select_subset(state, B);
			std::cout << "Selected B = ";
			for (const auto& b : B) {
				std::cout << b << " ";
			}
			std::cout << std::endl;

			std::vector<PolyDict> sygyzys;
			construct_sygyzys(state, B, sygyzys);
			std::cout << "Constructed sygyzys:" << std::endl;
			for (const auto& s : sygyzys) {
				std::cout << "\t" << s << std::endl;
			}

			std::set<MultiIndex> terms;
			construct_H(state, sygyzys, terms);
			std::cout << "Computed H:" << std::endl;
			for (const auto& p : sygyzys) {
				std::cout << "\t" << p << std::endl;
			}

			std::cout << "terms: ";
			for (const auto& t : terms) {
				std::cout << t << " ";
			}
			std::cout << std::endl;

			Eigen::MatrixXd M{sygyzys.size(), terms.size()};
			write_to_matrix(terms, sygyzys, M);

			std::cout << "Computed M:" << std::endl;
			std::cout << M << std::endl;

			reduce(M);

			std::cout << "Reduced M:" << std::endl;
			std::cout << M << std::endl;

			for (int i = 0; i < (int) sygyzys.size(); i++) {
				PolyDict poly = get_poly(terms, M, i);
				std::cout << "Checking " << poly << " ..." << std::endl;
				if (poly.is_zero()) {
					std::cout << "\t\t...is zero" << std::endl;
					continue;
				}
				if (leading_is_generated_by(poly, sygyzys)) {
					std::cout << "\t\t..." << poly.leading() << " is generated" << std::endl;
					continue;
				}
				std::cout << "\t\t...adding." << std::endl;
				state.add_polynomial(poly);
			}
		}
	}
}

#endif //IDEA_F4_H
