//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_GEOMETRY_H
#define GLVIEW_GEOMETRY_H

#include "./Maybe.h"

namespace billiards::geometry {

	[[nodiscard]] inline
	MaybeLine through(const MaybePoint& p1, const MaybePoint& p2) {
//		a * p1.x + b * p1.y + c == 0
//		a * p2.x + b * p3.y + c == 0
//		(p1.y - p2.y) * p1.x + (p2.x - p1.x) * p1.y + (p1.x * p2.y - p1.y * p2.x) == 0
//		p1.y * p1.x - p2.y * p1.x + p2.x * p1.y - p1.x * p1.y + p1.x * p2.y - p1.y * p2.x == 0
//		a * p2.x + b * p3.y + c == 0
		return MaybeLine{p1.y - p2.y, p2.x - p1.x, p1.x * p2.y - p1.y * p2.x};
	}

	[[nodiscard]] inline
	MaybeDouble distance_to(const MaybeLine& line, const MaybePoint& point) {
		auto l = line.normalize();
		return (l.c - l.a * point.x + l.b * point.y).abs();
	}

	[[nodiscard]] inline
	MaybeLine orthogonal_at(const MaybeLine& line, const MaybePoint& point)  {
		return MaybeLine{line.b, -line.a, -line.b * point.x + line.a * point.y};
	}

	[[nodiscard]] inline
	MaybeLine parallel_at(const MaybeLine& line, const MaybePoint& point) {
		return MaybeLine{line.a, line.b, -line.a * point.x - line.b * point.y};
	}

	[[nodiscard]] inline
	MaybePoint intersection(const MaybeLine& l1, const MaybeLine& l2) {
		auto hx = l1.b * l2.c - l1.c * l2.b;
		auto hy = l1.c * l2.a - l1.a * l2.c;
		auto hz = l1.a * l2.b - l1.b * l2.a;
		return MaybePoint{hx / hz, hy / hz};
	}

	[[nodiscard]] inline
	MaybePoint projection(const MaybeLine& line, const MaybePoint& point) {
		auto d = distance_to(line, point);
		if ((d.abs() < TOLERANCE).get_or(false)) {
			return point;
		}
		return intersection(line, orthogonal_at(line, point));
//
////		min (point.x - x) ** 2 + (point.y - y) ** 2
////		line.a * point.x + line.b * point.y + line.c == 0
//		return Point{};
	}

	[[nodiscard]] inline
	MaybePoint reflect(const MaybePoint& point, const MaybeLine& line) {
		auto p = projection(line, point);
		return point + (p - point) * 2;
	}



	[[nodiscard]] inline
	MaybePoint project_onto_segment(
		const MaybePoint& l1,
		const MaybePoint& l2,
		const MaybePoint& p
	) {
		const auto diff = l2 - l1;
		const auto t1 = (p - l1).dot(diff) / diff.norm2();
		const auto t2 = t1.max(0).min(1);
		return l1 + diff * t2;
	}

	[[nodiscard]] inline
	MaybeDouble distance_to_segment(
		const MaybePoint& l1,
		const MaybePoint& l2,
		const MaybePoint& p
	) {
		return (p - project_onto_segment(l1, l2, p)).norm();
	}
}

#endif //GLVIEW_GEOMETRY_H
