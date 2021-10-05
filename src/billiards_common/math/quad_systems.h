//
// Created by thallock on 10/3/21.
//

#ifndef IDEA_QUAD_SYSTEMS_H
#define IDEA_QUAD_SYSTEMS_H

#include <stdio.h>
#include <functional>
#include <iostream>
#include <cmath>
#include <list>

#include "billiards_common/math/polynomial.h"


namespace billiards::math {

	class UniqueSolutions2d {
	public:
		std::shared_ptr<std::vector<std::pair<double, double>>> solutions;
		const std::function<void(const double, const double)>& receiver;

		UniqueSolutions2d(const UniqueSolutions2d& unique)
			: solutions{unique.solutions}
			, receiver{unique.receiver} {}

		UniqueSolutions2d(const std::function<void(const double, const double)>& receiver)
			: solutions{std::make_shared<std::vector<std::pair<double, double>>>()}
			, receiver{receiver} {}

		void operator()(const double x, const double y) const {
			for (const auto& prev : *solutions) {
				if (std::abs(x - prev.first) < LARGER_TOL
					&& std::abs(y - prev.second) < LARGER_TOL) {
					return;
				}
			}
			solutions->emplace_back(x, y);
			receiver(x, y);
		}
	};


	class SolutionVerifier2d {
	public:
		const std::function<bool(const double, const double)>& does_solve;
		const std::function<void(const double, const double)>& receiver;

		SolutionVerifier2d(const SolutionVerifier2d& verifier)
			: does_solve{verifier.does_solve}
			, receiver{verifier.receiver} {}

		SolutionVerifier2d(
			const std::function<bool(const double, const double)>& does_solve,
			const std::function<void(const double, const double)>& receiver)
			: does_solve{does_solve}
			, receiver{receiver} {}
		~SolutionVerifier2d() = default;

		void operator()(const double x, const double y) const {
			if (!does_solve(x, y)) {
				return;
			}
			receiver(x, y);
		}
	};

// Solve:
// a00 + a10 * x + a20 * x ** 2 == 0
	void solve_20(
		const double a00, const double a10, const double a20,
		const double default_x, const double default_y,
		const std::function<void(const double, const double)>& receiver
	) {
		// Could use default_y
		solve_2(a00, a10, a20, default_x, [&](const double x) { receiver(x, default_y); });
	}

// Solve
// a00 + a10 * x + a01 * y + a20 * x ** 2 == 0
// TODO: There may be two solutions for default_y...
	void solve_21(
		const double a00, const double a10, const double a01, const double a20,
		const double default_x, const double default_y,
		const std::function<void(const double, const double)>& receiver
	) {
		if (std::abs(a01) < TOL) {
			solve_20(a00, a10, a20, default_x, default_y, receiver);
			return;
		}
		solve_1n(
			(a00 + a10 * default_x + a20 * default_x * default_x) / a01,
			[&](const double y) {
				receiver(default_x, y);
			});
	}

// Solve
// a00 + a10 * x + a01 * y + a20 * x ** 2 + a11 * x * y + a02 * y ** 2 == 0
/*
 * Sage:
x, y = var('x y')
a00, a10, a01, a20, a11, a02 = var('a00 a10 a01 a20 a11 a02')
poly = (a00 + a10 * x + a01 * y + a20 * x ** 2 + a11 * x * y + a02 * y ** 2).expand().collect(y)
(c, _), (b, _), (a, _) = poly.coefficients(y)
disc = (b^2 - 4 * a * c).expand().collect(x)
disc
convert_powers(disc)
convert_powers(c)
convert_powers(b)
convert_powers(a)

(dc, _), (db, _), (da, _) = disc.coefficients(x)
disc_disc = (db^2 - 4 * da * dc).expand()
convert_powers(disc_disc)
convert_powers(dc)
convert_powers(db)
convert_powers(da)
 */
	void solve_22(
		const double a00, const double a10, const double a01, const double a20, const double a11, const double a02,
		const double default_x, const double default_y,
		const std::function<void(const double, const double)>& receiver
	) {
		if (std::abs(a02) < TOL && std::abs(a11) < TOL) {
			solve_21(a00, a10, a01, a20, default_x, default_y, receiver);
			return;
		}

		// For what values of x is (a11^2 - 4*a02*a20)*x^2 + a01^2 - 4*a00*a02 - 2*(2*a02*a10 - a01*a11)*x positive?
		std::vector<double> roots;
		solve_2(
			std::pow(a01, 2) - 4*a00*a02, -4*a02*a10 + 2*a01*a11, std::pow(a11, 2) - 4*a02*a20, 0,
			[&](const double t) { roots.push_back(t); });
//		for (const double x : roots) {
//			const double disc = (std::pow(a11, 2) - 4*a02*a20)*std::pow(x, 2) + std::pow(a01, 2) - 4*a00*a02 - 2*(2*a02*a10 - a01*a11)*x;
//			std::cout << "discriminant should be 0: " << disc << std::endl;
//			std::cout << "==========================================++" << std::endl;
//		}
		std::list<double> points_to_try;
		points_to_try.push_back(default_x);
		if (roots.size() == 1) {
			points_to_try.push_back(roots[0] - 1);
			points_to_try.push_back(roots[0] + 1);
			points_to_try.push_back(roots[0]);
		}
		if (roots.size() == 2) {
			points_to_try.push_back(roots[0] - 1);
			points_to_try.push_back((roots[0] + roots[1]) / 2);
			points_to_try.push_back(roots[1] + 1);
			points_to_try.push_back(roots[0]);
			points_to_try.push_back(roots[1]);
		}

		for (const double x : points_to_try) {
			const double disc = (std::pow(a11, 2) - 4*a02*a20)*std::pow(x, 2) + std::pow(a01, 2) - 4*a00*a02 - 2*(2*a02*a10 - a01*a11)*x;
			if (disc < -TOL) {
				continue;
			}
			double const a = a02;
			double const b = a11*x + a01;
			if (disc < TOL) {
				receiver(x, -b / (2 * a));
				return;
			}
			const double d = std::sqrt(disc);
			receiver(x, (-b + d) / (2 * a));
			receiver(x, (-b + d) / (2 * a));
			return;
		}
	}


// Solve:
// a00 == 0
// b00 + b10 * x + b01 * y + b20 * x ** 2 + b11 * x * y + b02 * y ** 2 == 0
	void solve_00_22(
		const double a00,
		const double b00, const double b10, const double b01, const double b20, const double b11, const double b02,
		const double default_x, const double default_y,
		const std::function<void(const double, const double)>& receiver
	) {
		if (std::abs(a00) < TOL) {
			solve_22(b00, b10, b01, b20, b11, b02, default_x, default_y, receiver);
			return;
		}
	}

// Substitute x = x_val into
// a00 + a10 * x + a01 * y + a20 * x ** 2 + a11 * x * y + a02 * y ** 2 == 0
	void subs_22_x(
		const double x_val,
		const double a00, const double a10, const double a01, const double a20, const double a11, const double a02,
		const double default_y,
		const std::function<void(const double, const double)>& receiver
	) {
		solve_2(
			a00 + a10 * x_val + a20 * x_val * x_val, a01 + a11 * x_val, a02,
			default_y,
			[&](const double y) { receiver(x_val, y); }
		);
	}

// Substitute y = y_val into
// a00 + a10 * x + a01 * y + a20 * x ** 2 + a11 * x * y + a02 * y ** 2 == 0
	void subs_22_y(
		const double y_val,
		const double a00, const double a10, const double a01, const double a20, const double a11, const double a02,
		const double default_x,
		const std::function<void(const double, const double)>& receiver
	) {
		solve_2(
			a00 + a01 * y_val + a02 * y_val * y_val, a10 + a11 * y_val, a20,
			default_x,
			[&](const double x) { receiver(x, y_val); }
		);
	}

// Solve:
// a00 + a10 * x == 0
// b00 + b10 * x + b01 * y + b20 * x ** 2 + b11 * x * y + b02 * y ** 2 == 0
	void solve_01_22(
		const double a00, const double a10,
		const double b00, const double b10, const double b01, const double b20, const double b11, const double b02,
		const double default_x, const double default_y,
		const std::function<void(const double, const double)>& receiver
	) {
		if (std::abs(a10) < TOL) {
			solve_00_22(a00, b00, b10, b01, b20, b11, b02, default_x, default_y, receiver);
			return;
		}

		solve_1n(
			a00 / a10,
			[&](const double x) {
				subs_22_x(x, b00, b10, b01, b20, b11, b02, default_y, receiver);
			}
		);
	}

// Solve:
// a00 + a10 * x + a20 * x ** 2 == 0
// b00 + b10 * x + b01 * y + b20 * x ** 2 + b11 * x * y + b02 * y ** 2 == 0
	void solve_02_22(
		const double a00, const double a10, const double a20,
		const double b00, const double b10, const double b01, const double b20, const double b11, const double b02,
		double default_x, double default_y,
		const std::function<void(const double, const double)>& receiver
	) {
		if (std::abs(a20) < TOL) {
			solve_01_22(a00, a10, b00, b10, b01, b20, b11, b02, default_x, default_y, receiver);
			return;
		}

		solve_2n(
			a00 / a20, a10 / a20,
			[&](const double x) {
				subs_22_x(x, b00, b10, b01, b20, b11, b02, default_y, receiver);
			}
		);
	}

// Solve:
// a00 + a10 * x + a01 * y + a20 * x ** 2 == 0
// b00 + b10 * x + b01 * y + b20 * x ** 2 + b11 * x * y + b02 * y ** 2 == 0
/*
 *
x, y = var('x y')
a00, a10, a01, a20 = var('a00 a10 a01 a20')
b00, b10, b01, b20, b11, b02 = var('b00 b10 b01 b20 b11 b02')
eq1 = a00 + a10 * x + a01 * y + a20 * x ** 2
eq2 = b00 + b10 * x + b01 * y + b20 * x ** 2 + b11 * x * y + b02 * y ** 2

y_sol = solve(eq1 == 0, y)[0].right()
poly = eq2.substitute(y=y_sol).expand().collect(x)

print('\t\tconst double y = ' + convert_powers(y_sol) + ';')

for coef, ord in poly.coefficients(x):
	print('\t\tconst double q' + str(ord) + ' = ' + convert_powers(coef) + ';')

 */
	void solve_21_22(
		const double a00, const double a10, const double a01, const double a20,
		const double b00, const double b01, const double b10, const double b20, const double b11, const double b02,
		double default_x, double default_y,
		const std::function<void(const double, const double)>& receiver
	) {
		const double a012 = a01 * a01;
		if (std::abs(a01) < TOL || std::abs(a012) < TOL) {
			solve_02_22(a00, a10, a20, b00, b10, b01, b20, b11, b02, default_x, default_y, receiver);
			return;
		}

		const double q0 = b00 - a00*b01/a01 + std::pow(a00, 2)*b02/a012;
		const double q1 = -a10*b01/a01 + 2*a00*a10*b02/std::pow(a01, 2) + b10 - a00*b11/a01;
		const double q2 = -a20*b01/a01 + std::pow(a10, 2)*b02/a012 + 2*a00*a20*b02/a012 - a10*b11/a01 + b20;
		const double q3 = 2*a10*a20*b02/a012 - a20*b11/a01;
		const double q4 = std::pow(a20, 2)*b02/std::pow(a01, 2);
		solve_4(
			q0, q1, q2, q3, q4,
			default_x,
			[&](const double x) {
				const double y = -(a20*std::pow(x, 2) + a10*x + a00)/a01;
				receiver(x, y);
			}
		);
	}


// Solve:
// a0 * x^2 + p0 * x + q0 == 0
// a1 * x^2 + p1 * x + q1 == 0
	void solve_20_20(
		const double q0, const double p0, const double a0,
		const double q1, const double p1, const double a1,
		const double default_x,
		const std::function<void(const double)> receiver
	) {
		const auto check0 = [&](const double x) {
			if (std::abs(a0 * x * x + p0 * x + q0) < LARGER_TOL) {
				receiver(x);
			}
		};
		const auto check1 = [&](const double x) {
			if (std::abs(a1 * x * x + p1 * x + q1) < LARGER_TOL) {
				receiver(x);
			}
		};
		if (std::abs(a0) > TOL) {
			solve_2n(q0 / a0, p0 / a0, check1);
			return;
		}
		if (std::abs(a1) > TOL) {
			solve_2n(q1 / a1, p1 / a1, check0);
			return;
		}
		if (std::abs(p0) > TOL) {
			solve_1n(q0 / p0, check1);
		}
		if (std::abs(p1) > TOL) {
			solve_1n(q1 / p1, check0);
		}
		if (std::abs(q0) < TOL && std::abs(q1) < TOL) {
			return;
		}
		receiver(default_x);
	}


/*
 * Sage script:

# R.<x,y> = QQ[]
x, y = var('x y')
a0, b0, c0, d0, e0, f0 = var('a0 b0 c0 d0 e0 f0')
a1, b1, c1, d1, e1, f1 = var('a1 b1 c1 d1 e1 f1')

eq00 = f0 + d0 * x + e0 * y + a0 * x^2 + c0 * x * y + b0 * y^2
eq01 = f1 + d1 * x + e1 * y + a1 * x^2 + c1 * x * y + b1 * y^2

p0 = c0 * y + d0
q0 = f0 + e0 * y + b0 * y^2
p1 = c1 * y + d1
q1 = f1 + e1 * y + b1 * y^2

eq10 = a0 * x^2 + p0 * x + q0
eq11 = a1 * x^2 + p1 * x + q1

bool(eq00 == eq10) # True
bool(eq01 == eq11) # True

eq2 = (a1 * eq10 - a0 * eq11).expand().collect(x)

for coef, ord in eq2.coefficients(x):
	print('\t\t\tconst double l' + str(ord) + ' = ' + convert_powers(coef) + ';')

# x_sol = eq2.roots(x)[0][0]
x_sol = solve(eq2, x)[0].right()
num = numerator(x_sol)
den = denominator(x_sol)
l0, l1 = [p[0] for p in sorted(eq2.collect(x).coefficients(x), key=lambda p: p[1])]
bool(l0 + l1 * x == eq2)
bool(x_sol == num / den)

# print('\t\t\tconst double num = ' + convert_powers(num) + ';')
# print('\t\t\tconst double den = ' + convert_powers(den) + ';')

eq3 = (den^2 * eq10.substitute(x=x_sol)).simplify_full().expand().collect(y)
# eq3 = (a0 * num^2 + p0 * num * den + q0 * den^2).collect(y)

for coef, ord in eq3.coefficients(y):
	print('\t\tconst double q' + str(ord) + ' = ' + convert_powers(coef) + ';')

 */
//
// Solve:
// 		f0 + d0 * x + e0 * y + a0 * x ** 2 + c0 * x * y + b0 * y ** 2 == 0
// 		f1 + d1 * x + e1 * y + a1 * x ** 2 + c1 * x * y + b1 * y ** 2 == 0
//
	void solve_22_22(
		const double f0, const double d0, const double e0, const double a0, const double c0, const double b0,
		const double f1, const double d1, const double e1, const double a1, const double c1, const double b1,
		const double default_x, const double default_y,
		const std::function<void(const double, const double)>& o_receiver
	) {
		std::list<std::pair<double, double>> unique_solutions;
		const auto receiver = [&](const double x, const double y) {
			for (const auto& prev : unique_solutions) {
				if (std::abs(x - prev.first) < TOL && std::abs(y - prev.second) < TOL) {
					return;
				}
			}
			unique_solutions.emplace_back(x, y);
			o_receiver(x, y);
		};
		if (std::abs(b0) < TOL && std::abs(c0) < TOL) {
			solve_21_22(
				f0, d0, d0, a0,
				f0, d1, e1, a1, c1, d1,
				default_x, default_y,
				receiver
			);
			return;
		}
		const double q0 = -a0*a1*d0*d1*f0 + std::pow(a0, 2)*std::pow(d1, 2)*f0 + a0*std::pow(a1, 2)*std::pow(f0, 2) + a0*a1*std::pow(d0, 2)*f1 - std::pow(a0, 2)*d0*d1*f1 - 2*std::pow(a0, 2)*a1*f0*f1 + std::pow(a0, 3)*std::pow(f1, 2);
		const double q1 = -a0*a1*d0*d1*e0 + std::pow(a0, 2)*std::pow(d1, 2)*e0 + a0*a1*std::pow(d0, 2)*e1 - std::pow(a0, 2)*d0*d1*e1 - a0*a1*c1*d0*f0 - a0*a1*c0*d1*f0 + 2*std::pow(a0, 2)*c1*d1*f0 + 2*a0*std::pow(a1, 2)*e0*f0 - 2*std::pow(a0, 2)*a1*e1*f0 + 2*a0*a1*c0*d0*f1 - std::pow(a0, 2)*c1*d0*f1 - std::pow(a0, 2)*c0*d1*f1 - 2*std::pow(a0, 2)*a1*e0*f1 + 2*std::pow(a0, 3)*e1*f1;
		const double q2 = a0*a1*b1*std::pow(d0, 2) - a0*a1*b0*d0*d1 - std::pow(a0, 2)*b1*d0*d1 + std::pow(a0, 2)*b0*std::pow(d1, 2) - a0*a1*c1*d0*e0 - a0*a1*c0*d1*e0 + 2*std::pow(a0, 2)*c1*d1*e0 + a0*std::pow(a1, 2)*std::pow(e0, 2) + 2*a0*a1*c0*d0*e1 - std::pow(a0, 2)*c1*d0*e1 - std::pow(a0, 2)*c0*d1*e1 - 2*std::pow(a0, 2)*a1*e0*e1 + std::pow(a0, 3)*std::pow(e1, 2) + 2*a0*std::pow(a1, 2)*b0*f0 - 2*std::pow(a0, 2)*a1*b1*f0 - a0*a1*c0*c1*f0 + std::pow(a0, 2)*std::pow(c1, 2)*f0 - 2*std::pow(a0, 2)*a1*b0*f1 + 2*std::pow(a0, 3)*b1*f1 + a0*a1*std::pow(c0, 2)*f1 - std::pow(a0, 2)*c0*c1*f1;
		const double q3 = 2*a0*a1*b1*c0*d0 - a0*a1*b0*c1*d0 - std::pow(a0, 2)*b1*c1*d0 - a0*a1*b0*c0*d1 - std::pow(a0, 2)*b1*c0*d1 + 2*std::pow(a0, 2)*b0*c1*d1 + 2*a0*std::pow(a1, 2)*b0*e0 - 2*std::pow(a0, 2)*a1*b1*e0 - a0*a1*c0*c1*e0 + std::pow(a0, 2)*std::pow(c1, 2)*e0 - 2*std::pow(a0, 2)*a1*b0*e1 + 2*std::pow(a0, 3)*b1*e1 + a0*a1*std::pow(c0, 2)*e1 - std::pow(a0, 2)*c0*c1*e1;
		const double q4 = a0*std::pow(a1, 2)*std::pow(b0, 2) - 2*std::pow(a0, 2)*a1*b0*b1 + std::pow(a0, 3)*std::pow(b1, 2) + a0*a1*b1*std::pow(c0, 2) - a0*a1*b0*c0*c1 - std::pow(a0, 2)*b1*c0*c1 + std::pow(a0, 2)*b0*std::pow(c1, 2);

//		std::cout << "q0=" << q0 << std::endl;
//		std::cout << "q1=" << q1 << std::endl;
//		std::cout << "q2=" << q2 << std::endl;
//		std::cout << "q3=" << q3 << std::endl;
//		std::cout << "q4=" << q4 << std::endl;

		if (
			std::abs(q0) < TOL &&
			std::abs(q1) < TOL &&
			std::abs(q2) < TOL &&
			std::abs(q3) < TOL &&
			std::abs(q4) < TOL
		) {
			// They are a linear combination of each other...
			solve_22(
				f0, d0, e0, a0, c0, b0,
				default_x, default_y,
				receiver
			);
			return;
		}

		solve_4(
			q0, q1, q2, q3, q4,
			default_y,
			[&](const double y) {
//				std::cout << "\t\tquart 1: " << q0 + q1 * y + q2 * y * y + q3 * y * y * y + q4 * y * y * y * y
//						  << std::endl;

				const double l0 = a1*b0*std::pow(y, 2) - a0*b1*std::pow(y, 2) + a1*e0*y - a0*e1*y + a1*f0 - a0*f1;
				const double l1 = a1*c0*y - a0*c1*y + a1*d0 - a0*d1;
				const double pv0 = c0 * y + d0;
				const double qv0 = f0 + e0 * y + b0 * y * y;
				const double pv1 = c1 * y + d1;
				const double qv1 = f1 + e1 * y + b1 * y * y;

//				std::cout << "l1=" << l1 << std::endl;
//				std::cout << "l0=" << l0 << std::endl;
//				std::cout << "p=" << pv0 << std::endl;
//				std::cout << "q=" << qv0 << std::endl;
//				std::cout << "a0=" << a0 << std::endl;

				const auto rec = [&](const double x) {
//					std::cout << "\t\tlinear: " << l0 + l1 * x << std::endl;
//					std::cout << "\t\tquart 2: " << a0 * x * x + pv0 * x + qv0 << std::endl;
//					std::cout << "\t\tquart 3: " << a1 * x * x + pv1 * x + qv1 << std::endl;
//					std::cout << "\t\tinter linear: " << (
//						a1 * b0 * y * y - a0 * b1 * y * y + a1 * e0 * y - a0 * e1 * y + a1 * f0 - a0 * f1 +
//						(a1 * c0 * y - a0 * c1 * y + a1 * d0 - a0 * d1) * x
//					) << std::endl;
					receiver(x, y);
				};

				if (std::abs(l1) > TOL) {
					rec(-l0 / l1);
					return;
				}
				solve_20_20(
					qv0, pv0, a0,
					qv1, pv1, a1,
					default_x,
					rec
				);
			}
		);
	}
}

#endif //IDEA_QUAD_SYSTEMS_H
