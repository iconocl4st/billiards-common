//
// Created by thallock on 10/9/21.
//

#ifndef IDEA_HIGH_ORDER_POLYS_H
#define IDEA_HIGH_ORDER_POLYS_H

#include <iostream>
#include <Eigen/Dense>
#include <list>

#include "billiards_common/geometry/tolerance.h"

namespace billiards::shots::math {
	inline
	void compute_roots(
		const std::vector<double>& coefficients,
		std::list<double>& roots
	) {
		roots.clear();
		int degree = (int) coefficients.size() - 1;
		while (degree > 0 && std::abs(coefficients[degree]) < TOLERANCE) {
			--degree;
		}
		if (degree < 0) {
			return;
		} else if (degree == 0) {
			if (std::abs(coefficients[0]) < TOLERANCE) {
				roots.push_back(0);
			}
			return;
		}

		const double leading = coefficients[degree];
		Eigen::MatrixXd companion_matrix{degree, degree};
		for (int i = 0; i < degree; i++) {
			for (int j = 0; j < degree; j++) {
				if (j == degree - 1) {
					companion_matrix(i, j) = -coefficients[i] / leading;
				} else if (i == j + 1) {
					companion_matrix(i, j) = 1;
				} else {
					companion_matrix(i, j) = 0;
				}
			}
		}

		Eigen::EigenSolver<Eigen::MatrixXd> s(companion_matrix); // the instance s(A) includes the eigensystem
		std::cout << "Found matrix: " << std::endl;
		std::cout << companion_matrix << std::endl;

		const auto& eigen_values = s.eigenvalues();
		for (int i = 0; i < eigen_values.size(); i++) {
			const double real = eigen_values[i].real();
			const double imag = eigen_values[i].imag();
			if (std::abs(imag) < TOLERANCE) {
				roots.push_back(real);
			}
		}
	}
}



/*
 *
//		const double a0 = c0 / c6;
//		const double a1 = c1 / c6;
//		const double a2 = c2 / c6;
//		const double a3 = c3 / c6;
//		const double a4 = c4 / c6;
//		const double a5 = c5 / c6;
//
//		C	<<	0, 0, 0, 0, 0, -a0,
//				1, 0, 0, 0, 0, -a1,
//				0, 1, 0, 0, 0, -a2,
//				0, 0, 1, 0, 0, -a3,
//				0, 0, 0, 1, 0, -a4,
//				0, 0, 0, 0, 1, -a5;
		std::cout << companion_matrix << std::endl;
		std::cout << "eigenvalues:" << std::endl;
		std::cout << s.eigenvalues() << std::endl;
		std::cout << "eigenvectors=" << std::endl;
		std::cout << s.eigenvectors() << std::endl;
//		std::cout << s.eigenvalues()[0].
//		std::cout << s.eigenvalues()[0] << std::endl;
////		for (const auto v : s.eigenvalues()) {
////			std::cout << v << std::endl;
////		}
 */

#endif //IDEA_HIGH_ORDER_POLYS_H
