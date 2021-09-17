//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_GEOMETRY_H
#define GLVIEW_GEOMETRY_H

#include "./Point.h"
#include "./Line.h"

namespace billiards::geometry {

	inline
	std::optional<Line> through(std::optional<Point> p1, std::optional<Point> p2) {
		if (!p1 || !p2) {
			return {};
		}
//		a * p1.x + b * p1.y + c == 0
//		a * p2.x + b * p3.y + c == 0
//		(p1.y - p2.y) * p1.x + (p2.x - p1.x) * p1.y + (p1.x * p2.y - p1.y * p2.x) == 0
//		p1.y * p1.x - p2.y * p1.x + p2.x * p1.y - p1.x * p1.y + p1.x * p2.y - p1.y * p2.x == 0
//		a * p2.x + b * p3.y + c == 0
		return Line{p1->y - p2->y, p2->x - p1->x, p1->x * p2->y - p1->y * p2->x};
	}

	inline
	std::optional<double> distance_to(std::optional<Line> line, std::optional<Point> point) {
		if (!line || !point) {
			return {};
		}
		auto l = line->normalize();
		if (!l) {
			return {};
		}
		return std::abs(l->c - l->a * point->x + l->b * point->y);
	}

	inline
	std::optional<Line> orthogonal_at(std::optional<Line> line, std::optional<Point> point)  {
		if (!line || !point) {
			return {};
		}
		return Line{line->b, -line->a, -line->b * point->x + line->a * point->y};
	}

	inline
	std::optional<Point> intersection(std::optional<Line> l1, std::optional<Line> l2) {
		if (!l1 || !l2) {
			return {};
		}
		double hx = l1->b * l2->c - l1->c * l2->b;
		double hy = l1->c * l2->a - l1->a * l2->c;
		double hz = l1->a * l2->b - l1->b * l2->a;
		if (std::abs(hz) < TOLERANCE) {
			return {};
		}
		return Point{hx / hz, hy / hz};
	}

	inline
	std::optional<Point> projection(std::optional<Line> line, std::optional<Point> point) {
		std::optional<double> d = distance_to(line, point);
		if (!d) {
			return {};
		}
		if (std::abs(*d) < TOLERANCE) {
			return point;
		}
		return intersection(line, orthogonal_at(line, point));
//
////		min (point.x - x) ** 2 + (point.y - y) ** 2
////		line.a * point.x + line.b * point.y + line.c == 0
//		return Point{};
	}

	inline
	std::optional<Point> reflect(std::optional<Point> point, std::optional<Line> line) {
		if (!point || !line) {
			return {};
		}
		auto p = projection(line, point);
		if (!p) {
			return {};
		}
		return *point + (*p - *point) * 2;
	}
}

#endif //GLVIEW_GEOMETRY_H
