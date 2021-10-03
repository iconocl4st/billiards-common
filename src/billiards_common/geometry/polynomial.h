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

#include "billiards_common/geometry/Complex.h"

namespace billiards::math::poly {

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
		const double discriminant = c1 * c1 - 4 * c0;
		if (discriminant < 0) {
			return;
		}
		if (discriminant < TOL) {
			receiver(-c1 / 2);
			return;
		}
		const double d = std::sqrt(discriminant);
		receiver((-c1 + d) / 2);
		receiver((-c1 - d) / 2);
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
expand(c0 + c1 * y + c2 * y^2 + y^3).collect(x)
2/27*c2^3 + x^3 - 1/3*c1*c2 - 1/3*(c2^2 - 3*c1)*x + c0
*/
	inline
	void solve_3n(
		const double c0, const double c1, const double c2,
		const double default_value,
		const std::function<void(const double)>& receiver
	) {
		solve_3_simp(
			-1 / 3.0 * (c2 * c2 - 3 * c1), -(2 / 27.0 * c2 * c2 * c2 - 1 / 3.0 * c1 * c2 + c0),
			[c2, &receiver](double x) { receiver(x - c2 / 3); });
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
		solve_3n(c0 / c3, c1 / c3, c2 / c3, default_value, receiver);
	}

// Solve:
// a0 + a1 * x + a2 * x ** 2 + a3 * x ** 3 + x ** 4 == 0
	inline
	void solve_4n(
		const double a0, const double a1, const double a2, const double a3,
		const double default_val,
		const std::function<void(const double)>& receiver
	) {
		std::list<double> found_solutions;
		const std::function<void(const double)> check_receiver = [a0, a1, a2, a3, &receiver, &found_solutions](
			const double s) {
			if (std::abs(a0 + a1 * s + a2 * s * s + a3 * s * s * s + s * s * s * s) > LARGER_TOL) {
				return;
			}
			for (const double f: found_solutions) {
				if (std::abs(s - f) < LARGER_TOL) {
					return;
				}
			}
			found_solutions.push_back(s);
			receiver(s);
		};
		solve_3n(
			4 * a2 * a0 - a1 * a1 - a3 * a3 * a0, a1 * a3 - 4 * a0, -a2,
			default_val,
			[a0, a2, a3, &check_receiver](const double y1) {
				const double r1 = a3 * a3 - 4 * a2 + 4 * y1;
				const double r2 = y1 * y1 - 4 * a0;
				if (r1 < 0 || r2 < 0) {
					return;
				}
				const double sr1 = std::sqrt(r1);
				const double sr2 = std::sqrt(r2);
				solve_2n(0.5 * (y1 + sr2), 0.5 * (a3 + sr1), check_receiver);
				solve_2n(0.5 * (y1 + sr2), 0.5 * (a3 - sr1), check_receiver);
				solve_2n(0.5 * (y1 - sr2), 0.5 * (a3 + sr1), check_receiver);
				solve_2n(0.5 * (y1 - sr2), 0.5 * (a3 - sr1), check_receiver);
			}
		);
	}

// Solve:
// a0 + a1 * x + a2 * x ** 2 + a3 * x ** 3 + a4 * x ** 4 == 0
	inline
	void solve_4(
		const double a0, const double a1, const double a2, const double a3, const double a4,
		const double default_val,
		const std::function<void(const double)>& sol
	) {
		if (std::abs(a4) < TOL) {
			solve_3(a0, a1, a2, a3, default_val, sol);
			return;
		}
		solve_4n(a0 / a4, a1 / a4, a2 / a4, a3 / a4, default_val, sol);
	}
}

#endif //IDEA_POLYNOMIAL_H
