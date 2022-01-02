//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_VARIETYMATRIX_H
#define IDEA_VARIETYMATRIX_H

#include "algebra/vec/PolyVec.h"
#include "algebra/vec/Formatter.h"
#include <eigen3/Eigen/Dense>

namespace algebra::poly {

	namespace variety_matrix {

		inline
		int get_col(const std::vector<std::vector<int>>& mapping, const std::vector<int>& powers) {
			for (int i = 0; i < (int) mapping.size(); i++) {
				bool match = true;
				for (int j = 0; j < mapping[i].size() && match; j++) {
					if (powers[j] != mapping[i][j]) {
						match = false;
					}
				}
				if (match) {
					return i;
				}
			}
			return -1;
		}
		inline
		int ensure_col(std::vector<std::vector<int>>& mapping, const std::vector<int>& powers) {
			int index = get_col(mapping, powers);
			if (index >= 0) {
				return index;
			}
			index = (int) mapping.size();
			mapping.emplace_back(powers);
			return index;
		}

		inline
		int count_columns(std::vector<std::vector<int>>& mapping, const std::vector<const PolyVec*>& polynomials) {
			for (const auto& poly : polynomials) {
				for (const auto& term : poly->terms) {
					variety_matrix::ensure_col(mapping, term.powers);
				}
			}
			return (int) mapping.size();
		}
	}

	class VarietyMatrix {
	public:
		std::vector<std::vector<int>> mapping;
		int nrows;
		int ncols;
		Eigen::MatrixXd coefficients;

		explicit VarietyMatrix(const std::vector<const PolyVec*>& polynomials)
			: mapping{}
			, nrows{(int) polynomials.size()}
			, ncols{variety_matrix::count_columns(mapping, polynomials)}
			, coefficients{nrows, ncols}
		{
			for (int i = 0; i < nrows; i++) {
				for (int j = 0; j < ncols; j++) {
					coefficients(i, j) = 0;
				}
			}

			for (int i = 0; i < nrows; i++) {
				for (const auto& term : polynomials[i]->terms) {
					const int col = variety_matrix::get_col(mapping, term.powers);
					coefficients(i, col) = term.coefficient;
				}
			}
		}

		[[nodiscard]] inline
		Monomial at(int i, int j) const {
			return Monomial{coefficients(i, j), mapping[j]};
		}

		inline
		std::ostream& write_mapping(std::ostream& os, const Formatter& formatter) const {
			bool requires_comma = false;
			for (const std::vector<int>& powers: mapping) {
				if (requires_comma) {
					os << ", ";
				} else {
					requires_comma = true;
				}
				bool requires_mult = false;
				for (int i = 0; i < (int) powers.size(); i++) {
					if (powers[i] == 0) {
						continue;
					}
					if (requires_mult) {
						os << "*";
					} else {
						requires_mult = true;
					}
					os << formatter(i);
					if (powers[i] == 1) {
						continue;
					}
					os << "^" << powers[i];
				}
			}
			return os;
		}

		inline
		std::ostream& write(std::ostream& os, const Formatter& formatter) const {
			write_mapping(os, formatter) << "\n";
			for (int i = 0; i < nrows; i++) {
				bool requires_plus = false;
				for (int j = 0; j < ncols; j++) {
					if (std::abs(coefficients(i, j)) < POLY_TOL) {
						continue;
					}
					if (requires_plus) {
						os << " + ";
					} else {
						requires_plus = true;
					}

					at(i, j).write(os, formatter);
				}
				os << " == 0\n";
			}
			return os;
		}

		inline
		void parameterize() {

		}
	};

}

#endif //IDEA_VARIETYMATRIX_H
