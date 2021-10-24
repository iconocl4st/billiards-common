//
// Created by thallock on 10/21/21.
//

#ifndef IDEA_SPARSE_JORDAN_H
#define IDEA_SPARSE_JORDAN_H

#include <Eigen/Sparse>
#include <Eigen/SparseCore>

#include "algebra/Ideal.h"

namespace algebra::poly {

	[[nodiscard]] inline
	int get_pivot_row(Eigen::SparseMatrix<double, Eigen::RowMajor>& M, std::vector<int>& perms, int r, int c) {
		int max_r = r;
		double max_row_val = std::abs(M.coeff(perms[r], c));
		for (int i = r + 1; i < M.rows(); i++) {
			double alt_row_val = std::abs(M.coeff(perms[i], c));
			if (alt_row_val > max_row_val) {
				max_r = i;
				max_row_val = alt_row_val;
			}
		}
		return max_r;
	}

	inline
	void pivot(Eigen::SparseMatrix<double, Eigen::RowMajor>& M, std::vector<int>& perms, int r, int max_r) {
		if (max_r == r) {
			return;
		}
		int tmp = perms[r];
		perms[r] = perms[max_r];
		perms[max_r] = tmp;
	}

	inline
	void eliminate(Eigen::SparseMatrix<double, Eigen::RowMajor>& M, std::vector<int>& perms, int r, int c) {
		M.row(perms[r]) /= M.coeff(perms[r], c);
		for (int i = 0; i < M.rows(); i++) {
			if (i == r) {
				continue;
			}
			M.row(perms[i]) -= M.coeff(perms[i], c) * M.row(perms[r]);
//			if (std::abs(M.coeff(perms[i], c)) > 1e-4) {
//				throw std::runtime_error{"This should have been zero"};
//			}
//			M.coeffRef(perms[i], c) = 0;
//
//			for (int j = 0; j < M.cols(); j++) {
//				if (std::abs(M.coeff(perms[i], j)) < 1e-8) {
//					M.coeffRef(perms[i], j) = 0;
//				}
//			}
		}
	}

	inline
	void sparse_jordan(Eigen::SparseMatrix<double, Eigen::RowMajor>& M) {
		// Use eigen's permutation matrix...
		// complete orthogonal?

		std::vector<int> perms;
		perms.reserve(M.rows());
		for (int i = 0; i < (int) M.rows(); i++) {
			perms.push_back(i);
		}

		int c = 0;
		int r = 0;
		while (r < M.rows() && c < M.cols()) {
			int max_r = get_pivot_row(M, perms, r, c);
			pivot(M, perms, r, max_r);

			if (std::abs(M.coeff(perms[r], c)) < POLY_TOL) {
				++c;
				continue;
			}

			eliminate(M, perms, r, c);
			++r; ++c;

//			std::cout << "The new matrix: " << std::endl;
//			std::cout << M.toDense() << std::endl;
		}
	}
}

#endif //IDEA_SPARSE_JORDAN_H
