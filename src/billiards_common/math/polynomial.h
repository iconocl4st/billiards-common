//
// Created by thallock on 10/3/21.
//

#ifndef IDEA_POLYNOMIAL_H
#define IDEA_POLYNOMIAL_H

#include <stdio.h>

#include <functional>
#include <iostream>
#include <cmath>
#include <list>
#include <vector>

#include "billiards_common/math/Complex.h"
#include "billiards_common/math/newton.h"

namespace billiards::math {
	/*
	 * Common Sage:


docker run -it sagemath/sagemath:latest

import re
def convert_powers(expr):
	return re.sub(
	 	r'([a-zA-Z0-9]*)\^([0-9]+)', r'std::pow(\g<1>, \g<2>)', str(expr)
	).replace('/', '/ (double) ')

	 */


	class UniqueSolutions1d {
	public:
		std::shared_ptr<std::vector<double>> solutions;
		const std::function<void(const double)>& receiver;

		UniqueSolutions1d(const UniqueSolutions1d& unique)
			: solutions{unique.solutions}
			, receiver{unique.receiver} {}
		UniqueSolutions1d(const std::function<void(const double)>& receiver)
			: solutions{std::make_shared<std::vector<double>>()}
			, receiver{receiver} {}

		~UniqueSolutions1d() = default;

		void operator()(const double sol) const {
			for (const double& prev : *solutions) {
				if (std::abs(sol - prev) < LARGER_TOL) {
					return;
				}
			}
			solutions->push_back(sol);
			receiver(sol);
		}
	};

	class SolutionVerifier1d {
	public:
		const std::function<bool(const double)>& does_solve;
		const std::function<void(const double)>& receiver;

		SolutionVerifier1d(const SolutionVerifier1d& verifier)
			: does_solve{verifier.does_solve}
			, receiver{verifier.receiver} {}
		SolutionVerifier1d(const std::function<bool(const double)>& does_solve, const std::function<void(const double)>& receiver)
			: does_solve{does_solve}
			, receiver{receiver} {}
		~SolutionVerifier1d() = default;

		void operator()(const double sol) const {
			if (!does_solve(sol)) {
				return;
			}
			receiver(sol);
		}
	};


// Solve c == 0
	inline
	void solve_0(
		const double c,
		const double default_value,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(c) < TOL) {
			receiver(default_value);
		}
	}

// Solve c + x == 0
	inline
	void solve_1n(
		const double c,
		const std::function<void(const double)>& receiver
	) {
		receiver(-c);
	}

// Solve c0 + c1 * x == 0
	inline
	void solve_1(
		const double c0, const double c1,
		const double default_value,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(c1) < TOL) {
			solve_0(c0, default_value, receiver);
			return;
		}
		solve_1n(c0 / c1, receiver);
	}

// Solve c0 + c1 * x + x^2 == 0
	inline
	void solve_2n(
		const double c0, const double c1,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(c0) < TOL) {
			receiver(0);
			solve_1n(c1, receiver);
			return;
		}
		const double d = c1 * c1 - 4 * c0;
		const double discriminant = std::abs(d) < TOL ? 0 : d;
		if (discriminant < 0) {
			return;
		}
		if (discriminant < TOL) {
			receiver(-c1 / 2);
			return;
		}
		const double sd = std::sqrt(discriminant);
		receiver(sharpen_quadratic(c0, c1, 1.0, (-c1 + sd) / 2));
		receiver(sharpen_quadratic(c0, c1, 1.0, (-c1 - sd) / 2));
	}

// Solve c0 + c1 * x + c2 * x^2 == 0
	inline
	void solve_2(
		const double c0, const double c1, const double c2,
		const double default_value,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(c2) < TOL) {
			solve_1(c0, c1, default_value, receiver);
			return;
		}
		solve_2n(c0 / c2, c1 / c2, receiver);
	}

// Solve -Q + P * x + x^3 = 0
	inline
	void solve_3_simp(
		const double P, const double Q,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(Q) < TOL) {
			receiver(0);
			solve_2n(P, 0, receiver);
			return;
		}
		if (std::abs(P) < TOL) {
			receiver(std::cbrt(-Q));
			return;
		}

		// https://www.shsu.edu/kws006/professional/Concepts_files/SolvingCubics.pdf
		const double delta = std::pow(P / 3, 3) + std::pow(Q / 2, 2);
		const Complex b = Complex{delta}.pow(0.5) + (-Q / 2);
		const Complex beta = b.pow(1 / 3.0);
		const Complex alpha = Complex{P} / (beta * 3);
		const Complex omega = Complex{-0.5, std::sqrt(3) / 2};
		for (const Complex& sol: std::array<Complex, 3>{
			alpha - beta,
			alpha * omega * omega - beta * omega,
			alpha * omega - beta * omega * omega
		}) {
			if (sol.is_real()) {
				receiver(sol.real());
			}
		}
	}

/*
c0, c1, c2, x = var('c0 c1 c2 x')
y = x - c2 / 3
eq = expand(c0 + c1 * y + c2 * y^2 + y^3).collect(x)
# 2/27*c2^3 + x^3 - 1/3*c1*c2 - 1/3*(c2^2 - 3*c1)*x + c0

*/
	inline
	void solve_3n(
		const double c0, const double c1, const double c2,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(c0) < TOL) {
			receiver(0);
			solve_2n(c1, c2, receiver);
			return;
		}
		solve_3_simp(
			-1 / 3.0 * (c2 * c2 - 3 * c1), -(2 / 27.0 * c2 * c2 * c2 - 1 / 3.0 * c1 * c2 + c0),
			[&](double x) { receiver(x - c2 / 3); });
	}

// Solve c0 + c1 * x + c2 * x^2 + c3 * x^3 == 0
	inline
	void solve_3(
		const double c0, const double c1, const double c2, const double c3,
		const double default_value,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(c3) < TOL) {
			solve_2(c0, c1, c2, default_value, receiver);
			return;
		}
		solve_3n(c0 / c3, c1 / c3, c2 / c3, [&](const double s) {
			receiver(sharpen_cubic(c0, c1, c2, c3, s));
		});
	}

// Solve:
// a0 + a1 * x + a2 * x ** 2 + a3 * x ** 3 + x ** 4 == 0
	inline
	void solve_4n(
		const double a0, const double a1, const double a2, const double a3,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(a0) < TOL) {
			receiver(0);
			solve_3n(a1, a2, a3, receiver);
			return;
		}

		// Can't use the classes?
		std::list<double> unique_sols;
		const auto u_sols = [&](const double& sol) {
			for (const double prev : unique_sols) {
				if (std::abs(sol - prev) < LARGER_TOL) {
					return;
				}
			}
			unique_sols.push_back(sol);
			receiver(sol);
		};
		const auto verifier = [&](const double s) {
//			std::cout << "Hit " << s << std::endl;
//			std::cout << a0 << ", " << a1 << ", " << a2 << ", " << a3 << std::endl;
//			std::cout << a0 + a1 * s + a2 * s * s + a3 * s * s * s + s * s * s * s << std::endl;
			if (std::abs(a0 + a1 * s + a2 * s * s + a3 * s * s * s + s * s * s * s) < LARGER_TOL) {
//				std::cout << "Adding" << std::endl;
				u_sols(s);
			} else {
//				std::cout << "Not adding" << std::endl;
			}
		};
		solve_3n(
			4 * a2 * a0 - a1 * a1 - a3 * a3 * a0, a1 * a3 - 4 * a0, -a2,
			[&](const double y1) {
				const double r1a = a3 * a3 - 4 * a2 + 4 * y1;
				const double r2a = y1 * y1 - 4 * a0;
				const double r1 = std::abs(r1a) < LARGER_TOL ? 0 : r1a;
				const double r2 = std::abs(r2a) < LARGER_TOL ? 0 : r2a;
				if (r1 < 0 || r2 < 0) {
					return;
				}
				const double sr1 = std::sqrt(r1);
				const double sr2 = std::sqrt(r2);
				solve_2n(0.5 * (y1 + sr2), 0.5 * (a3 + sr1), verifier);
				solve_2n(0.5 * (y1 + sr2), 0.5 * (a3 - sr1), verifier);
				solve_2n(0.5 * (y1 - sr2), 0.5 * (a3 + sr1), verifier);
				solve_2n(0.5 * (y1 - sr2), 0.5 * (a3 - sr1), verifier);
			}
		);
	}

// Solve:
// a0 + a1 * x + a2 * x ** 2 + a3 * x ** 3 + a4 * x ** 4 == 0
	inline
	void solve_4(
		const double a0, const double a1, const double a2, const double a3, const double a4,
		const double default_val,
		const std::function<void(const double)>& receiver
	) {
		if (std::abs(a4) < TOL) {
			solve_3(a0, a1, a2, a3, default_val, receiver);
			return;
		}
		solve_4n(a0 / a4, a1 / a4, a2 / a4, a3 / a4, [&](const double s) {
			receiver(sharpen_quartic(a0, a1, a2, a3, a4, s));
		});
	}
}

#endif //IDEA_POLYNOMIAL_H
