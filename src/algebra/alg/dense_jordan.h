//
// Created by thallock on 10/21/21.
//

#ifndef IDEA_DENSE_JORDAN_H
#define IDEA_DENSE_JORDAN_H

#include <Eigen/Dense>

namespace algebra::poly {

	[[nodiscard]] inline
	int get_pivot_row(Eigen::MatrixXd& M, int r, int c) {
		int max_r = r;
		for (int i = r + 1; i < M.rows(); i++) {
			if (std::abs(M(i, c)) > std::abs(M(max_r, c))) {
				max_r = i;
			}
		}
		return max_r;
	}

	inline
	void pivot(Eigen::MatrixXd& M, std::vector<int>& pivots, int r, int max_r) {
		if (max_r == r) {
			return;
		}
		int tmp = pivots[r];
		pivots[r] = pivots[max_r];
		pivots[max_r] = tmp;

		M.row(r).swap(M.row(max_r));
	}

	inline
	void eliminate(Eigen::MatrixXd& M, int r, int c) {
		M.row(r) /= M(r, c);
		for (int i = 0; i < M.rows(); i++) {
			if (i == r) {
				continue;
			}
			M.row(i) -= M(i, c) * M.row(r);
		}
	}

	inline
	void dense_jordan(Eigen::MatrixXd& M) {
		// Use eigen's permutation matrix...
		// TODO: Don't actually need to keep the pivots
		// complete orthogonal?
		// There really ought to be an efficient (built-in) method for this...
		std::vector<int> pivots;
		for (int i = 0; i < M.rows(); i++) {
			pivots.push_back(i);
		}
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

			if (std::abs(M(r, c)) < POLY_TOL) {
				++c;
				continue;
			}

			eliminate(M, r, c);
			++r; ++c;
		}
	}
}

#endif //IDEA_DENSE_JORDAN_H
