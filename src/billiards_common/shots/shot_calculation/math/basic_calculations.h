//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SIMPLE_CALCULATIONS_H
#define IDEA_SIMPLE_CALCULATIONS_H

#include "billiards_common/geometry/geometry.h"

namespace billiards::geometry {

	[[nodiscard]] inline
	MaybeBool on_same_side(
		const MaybePoint& s1, const MaybePoint& s2,
		const MaybePoint& p1, const MaybePoint& p2
	) {
		auto line = through(s1, s2);
		auto d1 = line.at(p1);
		auto d2 = line.at(p2);
		return (
			d1.abs() < TOLERANCE ||
			d2.abs() < TOLERANCE ||
			d1.signum() == d2.signum()
		);
	}

// distance to project onto point?
	[[nodiscard]] inline
	MaybeBool points_are_colinear(const MaybePoint& p1, const MaybePoint& p2, const MaybePoint& p3) {
		const auto s1 = p1.y - p2.y;
		const auto s2 = p2.x - p1.x;
		const auto s3 = p1.x * p2.y - p1.y * p2.x;
//		if (std::abs(s3) < TOL) {
//			//
//		}
		const auto d = p3.x * s1 + p3.y * s2 + s3;
		return d.abs() < TOLERANCE;
	}

	[[nodiscard]] inline
	MaybePoint extend_line(const MaybePoint& p1, const MaybePoint& p2, const MaybeDouble& d) {
		auto diff = p2 - p1;
		return p1 + diff * (d / diff.norm() + 1);
	}


	[[nodiscard]] inline
	MaybeDouble determinant(const MaybePoint& p1, const MaybePoint& p2) {
		return p1.x * p2.y - p1.y * p2.x;
	}

	[[nodiscard]] inline
	MaybeBool is_to_the_right_of(const MaybePoint& p1, const MaybePoint& p2) {
		return determinant(p1, p2) < 0;
	}

	// TODO: intersections...
	[[nodiscard]] inline
	MaybeBool triangle_contains2(
		const MaybePoint& v1,
		const MaybePoint& v2,
		const MaybePoint& v3,
		const MaybePoint& p
	) {
		return (
			on_same_side(v1, v2, v3, p) &&
			on_same_side(v1, v3, v2, p) &&
			on_same_side(v2, v3, v1, p)
		);
	}
}

#endif //IDEA_SIMPLE_CALCULATIONS_H
