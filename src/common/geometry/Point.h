//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_POINT_H
#define GLVIEW_POINT_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>

#include "../utils/Serializable.h"
#include "./tolerance.h"

namespace billiards::geometry {

class Point : public billiards::json::Serializable  {
public:
	double x, y;

	Point(double x, double y) : x{x}, y{y} {}
	Point() : Point{0, 0} {}
//	Point(const Point& p) : x{p.x}, y{p.y} {}

	[[nodiscard]] double norm2() const { return x * x + y * y; }
	[[nodiscard]] double norm() const { return std::sqrt(norm2()); }

	void parse(const nlohmann::json& value) override {
		if (value.contains("x") && value["x"].is_number()) {
			x = value["x"].get<double>();
		}
		if (value.contains("y") && value["y"].is_number()) {
			y = value["y"].get<double>();
		}
	}
	void to_json(billiards::json::SaxWriter& writer) const override {
		writer.begin_object();
		writer.field("x", x);
		writer.field("y", y);
		writer.end_object();
	}

	[[nodiscard]] Point min(const Point& other) const { return Point{std::min(x, other.x), std::min(y, other.y)}; }
	[[nodiscard]] Point max(const Point& other) const { return Point{std::max(x, other.x), std::max(y, other.y)}; }

	Point operator-(const Point& other) const { return Point{x - other.x, y - other.y}; }
	Point operator+(const Point& other) const { return Point{x + other.x, y + other.y}; }
	Point operator/(double val) const { return Point{x / val, y / val}; }
	[[nodiscard]] std::optional<Point> safe_divide(double val) const {
		if (std::abs(val) < TOLERANCE) {
			return {};
		}
		return Point{x / val, y / val};
	}
	Point operator*(double val) const { return Point{x * val, y * val}; }
//	void operator=(const Point& other) { x = other.x; y = other.y; }

	friend
	std::ostream& operator<<(std::ostream& os, const Point& p) {
		os << "{x:" << p.x << ",y:" << p.y << "}";
		return os;
	}
};

}


#endif //GLVIEW_POINT_H
