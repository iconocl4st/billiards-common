//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_GEOMETRY_H
#define GLVIEW_GEOMETRY_H

#include "./Maybe.h"

namespace billiards::geometry {

	inline
	MaybeLine through(const MaybePoint& p1, const MaybePoint& p2) {
//		a * p1.x + b * p1.y + c == 0
//		a * p2.x + b * p3.y + c == 0
//		(p1.y - p2.y) * p1.x + (p2.x - p1.x) * p1.y + (p1.x * p2.y - p1.y * p2.x) == 0
//		p1.y * p1.x - p2.y * p1.x + p2.x * p1.y - p1.x * p1.y + p1.x * p2.y - p1.y * p2.x == 0
//		a * p2.x + b * p3.y + c == 0
		return MaybeLine{p1.y - p2.y, p2.x - p1.x, p1.x * p2.y - p1.y * p2.x};
	}

	inline
	MaybeDouble distance_to(const MaybeLine& line, const MaybePoint& point) {
		auto l = line.normalize();
		return (l.c - l.a * point.x + l.b * point.y).abs();
	}

	inline
	MaybeLine orthogonal_at(const MaybeLine& line, const MaybePoint& point)  {
		return MaybeLine{line.b, -line.a, -line.b * point.x + line.a * point.y};
	}

	inline
	MaybePoint intersection(const MaybeLine& l1, const MaybeLine& l2) {
		auto hx = l1.b * l2.c - l1.c * l2.b;
		auto hy = l1.c * l2.a - l1.a * l2.c;
		auto hz = l1.a * l2.b - l1.b * l2.a;
		return MaybePoint{hx / hz, hy / hz};
	}

	inline
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

	inline
	MaybePoint reflect(const MaybePoint& point, const MaybeLine& line) {
		auto p = projection(line, point);
		return point + (p - point) * 2;
	}
}

#endif //GLVIEW_GEOMETRY_H
