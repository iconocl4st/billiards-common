//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_GEOMETRY_H
#define GLVIEW_GEOMETRY_H

#include "./Point.h"
#include "./Line.h"

namespace billiards::geometry {

	typedef std::optional<const Line&> Line OLineArg;
	typedef std::optional<const Point&> Point OPointArg;

	typedef std::optional<Line> Line OLine;
	typedef std::optional<Point> Point OPoint;

	std::optional<double> distance_to(const OLineArg& line, const OPointArg& point) {
		if (!line || !point) {
			return {};
		}
		auto l = line->normalize();
		if (!l) {
			return {};
		}
		return std::abs(l->c - l->a * point.x + l->b * point.y);
	}
	std::optional<Point> projection(const Line& line, const Point& point) {
		std::optional<double> d = distance_to(line, point);
		if (!d) {
			return {};
		}
		if (std::abs(*d) < TOLERANCE) {
			return point;
		}
		Line orth = orthogonal_at(line, point);
		auto inter = intersection();

//		min (point.x - x) ** 2 + (point.y - y) ** 2
//		line.a * point.x + line.b * point.y + line.c == 0
		return Point{};
	}

	inline
	std::optional<Point> intersection(const Line& l1, const Line& l2) {
		double hx = l1.b * l2.c - l1.c * l2.b;
		double hy = l1.c * l2.a - l1.a * l2.c;
		double hz = l1.a * l2.b - l1.b * l2.a;
		if (std::abs(hz) < TOLERANCE) {
			return {};
		}
		return Point{hx / hz, hy / hz};
	}

	inline
	Line orthogonal_at(const Line &line, const Point &point)  {
		return Line{line.b, -line.a, -line.b * point.x + line.a * point.y};
	}

	std::optional<Point> reflect(const Point &point, const Line& line) {
	}

	inline
	Line through(const Point &p1, const Point &p2) {
//		a * p1.x + b * p1.y + c == 0
//		a * p2.x + b * p3.y + c == 0
//		(p1.y - p2.y) * p1.x + (p2.x - p1.x) * p1.y + (p1.x * p2.y - p1.y * p2.x) == 0
//		p1.y * p1.x - p2.y * p1.x + p2.x * p1.y - p1.x * p1.y + p1.x * p2.y - p1.y * p2.x == 0
//		a * p2.x + b * p3.y + c == 0
		return Line{
			p1.y - p2.y,
			p2.x - p1.x,
			p1.x * p2.y - p1.y * p2.x
		};
	}
}

#endif //GLVIEW_GEOMETRY_H
