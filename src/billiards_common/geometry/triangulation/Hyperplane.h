//
// Created by thallock on 12/22/21.
//

#ifndef IDEA_HYPERPLANE_H
#define IDEA_HYPERPLANE_H

#include <cmath>

#include "billiards_common/geometry/tolerance.h"

namespace billiards::geometry {

	class Hyperplane {
	public:
		// All lines with a * x + b * y + c == 0
		double a, b, c;

		Hyperplane() : a{0}, b{0}, c{0} {}

//		[[nodiscard]] inline
//		double at(const Point &p) const {
//			return a * p.x + b * p.y + c;
//		}

		[[nodiscard]] inline
		double at(const double x, const double y) const {
			return a * x + b * y + c;
		}

		[[nodiscard]] inline
		bool contains(const double x, const double y) const {
			return at(x, y) < TOLERANCE;
		}

//		friend inline
//		std::ostream &operator<<(std::ostream &o, const Hyperplane &h) {
//			return o << h.a << " * x + " << h.b << " * y + " << h.c << " == 0";
//		}

		inline
		bool orient(
				const double v1x, const double v1y, // A point on the line
				const double v2x, const double v2y, // A point on the line
				const double v3x, const double v3y  // A point feasible with respect to the line
		) {
			const double ax = v2y - v1y;
			const double ay = -(v2x - v1x);
			const double ac = (v2x - v1x) * v1y - (v2y - v1y) * v1x;

			const double n = std::sqrt(ax * ax + ay * ay);
			if (n < TOLERANCE) {
				return false;
			}

			const double bx = ax / n;
			const double by = ay / n;
			const double bc = ac / n;

			const double val = bx * v3x + by * v3y + bc;

			if (val < -TOLERANCE) {
				a = bx;
				b = by;
				c = bc;
			} else if (val > TOLERANCE) {
				a = -bx;
				b = -by;
				c = -bc;
			} else {
				return false;
			}

			return true;
		}

//		inline
//		bool orient(const Point &v1, const Point &v2, const Point &o) {
//			return orient(v1.x, v1.y, v2.x, v2.y, o.x, o.y);
//		}




#if 0
		[[nodiscard]] inline
	bool updateMap(
		const config::TableDimensions& dims,
		const config::TableGeometry& geometry
	) {
//		table_dims = dims;
//		of = geometry.lower_left();
//		up = geometry.upper_left() - geometry.lower_left();
//		rt = geometry.lower_right() - geometry.lower_left();
		return construct_map(dims, geometry, tri_map);
	}

	[[nodiscard]] inline
	geometry::Point map(const double x, const double y) const {
		return tri_map.map(x, y);

//		if (success) {
//			return ret;
//		}
//		double xn = x / table_dims.width;
//		double yn = y / table_dims.height;
//		return geometry::Point{
//			of.x + rt.x * xn + up.x * yn,
//			of.y + rt.y * xn + up.y * yn
//		};
	}


		[[nodiscard]] inline
	geometry::Point unmap(const double x, const double y) const {
		const double det = rt.x * up.y - up.x * rt.y;
		if (std::abs(det) < TOLERANCE) {
			return geometry::Point{};
		}
		const double xs = x - of.x;
		const double ys = y - of.y;
		const double xn = (+up.y * xs - up.x * ys) / det;
		const double yn = (-rt.y * xs + rt.x * ys) / det;

		return geometry::Point{xn * table_dims.width, yn * table_dims.height};
	}

	[[nodiscard]] inline
	geometry::Point unmap(const geometry::Point& in) const {
		return unmap(in.x, in.y);
	}
	[[nodiscard]] inline
	geometry::Point map(const geometry::Point& in) const {
		return map(in.x, in.y);
	}
#endif
	};

}

#endif //IDEA_HYPERPLANE_H
