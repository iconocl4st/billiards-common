//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SHOT_CALCULATIONS_H
#define IDEA_SHOT_CALCULATIONS_H

#include "math/basic_calculations.h"
#include "billiards_common/math/linear_system_2x2.h"

namespace billiards::geometry {
	// TOOD: remove this file...



	[[nodiscard]] inline
	MaybePoint calculate_rolling_glance(
		const MaybePoint& src,
		const MaybeDouble& src_r,
		const MaybePoint& obj,
		const MaybeDouble& obj_r,
		const MaybePoint& dst
	) {
		// Solve:
		// (x - obj.x) ** 2 + (y - obj.y) ** 2 == (obj_r + src_r) ** 2
		// p = {x, y}

		// aim = p - src
		// dir = dst - p
		// tan = orthogonal aim at p
		// tan @ tan == 1
		//
		// dir == 2.0 / 7 * tan + 5.0 / 7 * t * aim
		// (t * aim - tan) @ tan == 0
		// t == 1 / (aim @ tan)

		// dir == 2.0 / 7 * tan + 5.0 / 7 * aim / (aim @ tan)
		// dir == 2.0 / 7 * tan + 5.0 / 7 * aim / (aim @ tan)

		// x, y
		// aim = {src.y - y, x - src.x, y * (src.x - x) + x * (y - src.y)} // runs through x,y
		// tan = {x - src.x, y - src.y, x * (src.x - x) + y * (src.y - y)} // runs though x, y; orthogonal to aim


		// d = 2/7 * n + 5/7 * t * a
		// (t * a - n) @ n == 0
		// t * a @ n == n @ n


		// a = p - src
		// n @ a == 0
		// n @ [p; 1] == 0
		// d = 5/7 * n @ n + 1 / (n @ a) * a + 2/7 * n
		// d = dst - p

		// nx * (x - sx) + ny * (y - sy) == 0
		// dst.x - p == 2/7 * nx + 5/7 * (nx ** 2 + ny ** 2 +  1 / (nx * (x - sx) + ny * (y - sy)) *
		return MaybePoint{};
	}

}


#endif //IDEA_SHOT_CALCULATIONS_H
