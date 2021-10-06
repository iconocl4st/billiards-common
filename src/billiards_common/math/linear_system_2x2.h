//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_2_LINEAR_SYSTEM_H
#define IDEA_2_LINEAR_SYSTEM_H

#include "billiards_common/geometry/Maybe.h"

namespace billiards::geometry {


	// Any solution to:
	// a * x + b * y + c == 0
	[[nodiscard]] inline
	MaybePoint any_solution(
		const double a, const double b, const double c,
		const double default_x, const double default_y
	) {
		const bool a_is_small = std::abs(a) < TOLERANCE;
		const bool b_is_small = std::abs(b) < TOLERANCE;
		const bool c_is_small = std::abs(c) < TOLERANCE;

		if (a_is_small && b_is_small && c_is_small) {
			return MaybePoint{default_x, default_y};
		} else if (a_is_small && b_is_small) {
			return MaybePoint{};
		} else if (a_is_small) {
			return MaybePoint{default_x, -c / b};
		} else if (b_is_small) {
			return MaybePoint{-c / a, default_y};
		} else {
			// Could use the larger value...
			const double sol1x = default_x;
			const double sol1y = (-c - a * default_x) / b;
			const double sol2x = default_y;
			const double sol2y = (-c - b * default_y) / a;
			return MaybePoint{(sol1x + sol2x) / 2, (sol1y + sol2y) / 2};
		};
	}

	// Solve:
	// r00 * x + r01 * y + r02 == 0
	//           r11 * y + r12 == 0
	[[nodiscard]] inline
	MaybePoint solve(
		const double r00, const double r01, const double r02,
		                  const double r11, const double r12,
		const double default_x, const double default_y
	) {
		if (std::abs(r11) < TOLERANCE) {
			if (std::abs(r12) < TOLERANCE) {
				return any_solution(r00, r01, r02, default_x, default_y);
			} else {
				return MaybePoint{};
			}
		} else {
			const auto y = -r12 / r11;
			if (std::abs(r00) < TOLERANCE) {
				if (std::abs(r02 - r01 * y) < TOLERANCE) {
					return MaybePoint{default_x, y};
				} else {
					return MaybePoint{};
				}
			} else {
				// r00 * x + r01 * y + r02 == 0
				return MaybePoint{(-r02 - r01 * y) / r00, y};
			}
		}
	}

#if 1
	// Solve:
	// a00 * x + a01 * y + a02 == 0
	// a10 * x + a11 * y + a12 == 0
	// according to
	// http://www.seas.ucla.edu/~vandenbe/133A/lectures/qr.pdf
	[[nodiscard]] inline
	MaybePoint solve_2x2(
		const double a00, const double a01, const double a02,
		const double a10, const double a11, const double a12,
		const double default_x, const double default_y
	) {
		const auto ny = std::sqrt(a00 * a00 + a10 * a10);
		const auto w0 = a00 - ny;
		const auto w1 = a01;
		const auto nw = std::sqrt(w0 * w0 + w1 * w1);
		if (std::abs(nw) < TOLERANCE) {
			// y = |y| e1
//			if (std::abs(a10) > TOLERANCE) {
//				throw std::runtime_error{"This should have been 0..."};
//			}
			return solve(
				a00, a01, a02,
				     a11, a12,
				default_x, default_y);
		}
		const auto v0 = w0 / nw;
		const auto v1 = w1 / nw;
		const auto v00 = v0 * v0;
		const auto v01 = v0 * v1;
		const auto v11 = v1 * v1;
		const auto r00 = a00 - 2 * (v00 * a00 + v01 * a10);
		const auto r01 = a01 - 2 * (v00 * a01 + v01 * a11);
		const auto r02 = a02 - 2 * (v00 * a02 + v01 * a12);
		const auto r10 = a10 - 2 * (v01 * a00 + v11 * a10);
		const auto r11 = a11 - 2 * (v01 * a01 + v11 * a11);
		const auto r12 = a12 - 2 * (v01 * a02 + v11 * a12);

//		if (std::abs(r10) > TOLERANCE) {
//			throw std::runtime_error{"This should have been 0..."};
//		}
		return solve(
			r00, r01, r02,
			     r11, r12,
			default_x, default_y);
	}
#else

	[[nodiscard]] inline
	MaybePoint solve(
		const MaybeDouble& a00, const MaybeDouble& a01, const MaybeDouble& b0,
		const MaybeDouble& a10, const MaybeDouble& a11, const MaybeDouble& b1
	) {
		const bool a00_is_small = a00.abs() < TOLERANCE;
		const bool a01_is_small = a01.abs() < TOLERANCE;
		const bool a10_is_small = a10.abs() < TOLERANCE;
		const bool a11_is_small = a11.abs() < TOLERANCE;
		const bool b0_is_small = b0.abs() < TOLERANCE;
		const bool b1_is_small = b1.abs() < TOLERANCE;

		if (a00_is_small && a01_is_small) {
			if (b0_is_small) {

			} else {
				return MaybePoint{};
			}
		}
			// solve c == 0...

		} else if (a_is_small) {
			// (x - src.x) / (y - src.y) == (x - dst.x) / (y - dst.y)
			// (y - dst.y) * x - (y - dst.y) * src.x == (y - src.y) * x - (y - src.y) * dst.x
			// (y - dst.y) * x - (y - src.y) * x == (y - dst.y) * src.x - (y - src.y) * dst.x
			// [src.y - dst.y] * x == (y - dst.y) * src.x - (y - src.y) * dst.x
			const auto y = -c / bank_line.b;
			const auto num = (y - dst.y) * src.x - (y - src.y) * dst.x;
			if (dy_is_small) {
				// x can be anything
				return MaybePoint{(src.x + dst.x) / 2.0, y};
			} else {
				return MaybePoint{num / dy, y};
			}
		} else if (b_is_small) {
			// (y - src.y) / (x - src.x) == (y - dst.y) / (x - dst.x)
			// (x - dst.x) * y - (x - dst.x) * src.y == (x - src.x) * y - (x - src.x) * dst.y
			// (x - dst.x) * y - (x - src.x) * y == (x - dst.x) * src.y - (x - src.x) * dst.y
			// [src.x - dst.x] * y == (x - dst.x) * src.y - (x - src.x) * dst.y
			const auto x = -c / bank_line.a;
			const auto num = (x - dst.x) * src.y - (x - src.x) * dst.y;
			if (dx_is_small) {
				// y can be anything
				return MaybePoint{x, (src.y + dst.y) / 2.0};
			} else {
				return MaybePoint{x, num / dx};
			}
		} else {
	}
#endif



	[[nodiscard]] inline
	MaybePoint solve_2x2(
		const MaybeDouble& a00, const MaybeDouble& a01, const MaybeDouble& a02,
		const MaybeDouble& a10, const MaybeDouble& a11, const MaybeDouble& a12,
		const MaybeDouble& default_x, const MaybeDouble& default_y
	) {
		return solve_2x2(
			a00.get(), a01.get(), a02.get(),
			a10.get(), a11.get(), a02.get(),
			default_x.get(), default_y.get());
	};
}


#endif //IDEA_2_LINEAR_SYSTEM_H
