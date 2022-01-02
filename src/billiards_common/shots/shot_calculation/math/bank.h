//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_BANK_H
#define IDEA_BANK_H


#include "billiards_common/shots/shot_calculation/math/basic_calculations.h"
#include "billiards_common/geometry/Maybe.h"
#include "billiards_common/math/linear_system_2x2.h"

namespace billiards::shots::math {

	[[nodiscard]] inline
	geometry::MaybePoint calculate_bank(
		const geometry::MaybePoint& src, const geometry::MaybePoint& dst,
		const geometry::MaybePoint& s1, const geometry::MaybePoint& s2,
		const geometry::MaybePoint& in,
		const geometry::MaybeDouble radius
	) {
		const auto rail_line = geometry::through(s1, s2);
		const auto reflection = geometry::reflect(dst, rail_line);
		const auto travel_line = geometry::through(src, reflection);
		const auto bank_line = geometry::parallel_at(rail_line, s1 + in * radius);
		const auto intersection = geometry::intersection(bank_line, travel_line);
		return intersection;
	}


	[[nodiscard]] inline
	geometry::MaybePoint calculate_bank_2(
		const geometry::MaybePoint& src, const geometry::MaybePoint& dst,
		const geometry::MaybePoint& s1, const geometry::MaybePoint& s2,
		const geometry::MaybePoint& in,
		const geometry::MaybeDouble radius
	) {
		const auto rail_line = through(s1, s2);
		const auto bank_line = parallel_at(rail_line, s1 + in * radius);

		// Solve:
		// (x - src.x) / (y - src.y) == (x - dst.x) / (y - dst.y)
		// a * x + b * y + c == 0

		// (y - dst.y) * (x - src.x) == alpha * (x - dst.x) * (y - src.y) is nonlinear

		// (y - dst.y) * (x - src.x) == (x - dst.x) * (y - src.y)
		// y * (x - src.x) - dst.y * (x - src.x) == x * (y - src.y) - dst.x * (y - src.y)
		// y * x - y * src.x - dst.y * x + dst.y * src.x == x * y - x * src.y - dst.x * y + dst.x * src.y
		// y * x - y * src.x - dst.y * x + dst.y * src.x - x * y + x * src.y + dst.x * y - dst.x * src.y == 0
		// -y * src.x - dst.y * x + dst.y * src.x + x * src.y + dst.x * y - dst.x * src.y == 0
		// [src.y - dst.y] * x - [src.x - dst.x] * y + [dst.y * src.x - dst.x * src.y] == 0

		return solve_2x2(
			rail_line.a, rail_line.b, rail_line.c,
			src.y - dst.y, src.x - dst.x, dst.y * src.x - dst.x * src.y,
			(src.x + dst.x) / 2, (src.y + dst.y) / 2
		);
	}


}
#endif //IDEA_BANK_H
