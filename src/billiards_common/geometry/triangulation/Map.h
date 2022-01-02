//
// Created by thallock on 12/22/21.
//

#ifndef IDEA_MAP_H
#define IDEA_MAP_H

#include <array>

#include "billiards_common/geometry/triangulation/Hyperplane.h"
#include "billiards_common/geometry/geometry.h"

namespace billiards::geometry {

	// TODO: Move this to the projection repo?

	class TriangularMap {
	public:
		std::array<Hyperplane, 3> domain;
		std::array<Point, 3> vertices;
		double a[2][3];

		inline
		geometry::Point map(const double x1, const double x2) const {
			return Point{
				a[0][2] + a[0][0] * x1 + a[0][1] * x2,
				a[1][2] + a[1][0] * x1 + a[1][1] * x2
			};
		}

		[[nodiscard]] inline
		bool contains(const double x, const double y) const {
			return (
				domain[0].contains(x, y) &&
				domain[1].contains(x, y) &&
				domain[2].contains(x, y)
			);
		}

		[[nodiscard]] inline
		bool contains(const Point& p) const {
			return contains(p.x, p.y);
		}

		[[nodiscard]] inline
		MaybeDouble distance_to_edge(const double x, const double y) const {
			MaybePoint p{x, y};
			auto d1 = distance_to_segment(vertices[0], vertices[1], p);
			auto d2 = distance_to_segment(vertices[1], vertices[2], p);
			auto d3 = distance_to_segment(vertices[2], vertices[0], p);
			return d1.min(d2).min(d3);
		}
	};

	class TriangulationMap {
	public:
		std::array<TriangularMap, 10> maps;

		[[nodiscard]] inline
		Point map(const double x, const double y) const {
			for (auto& map : maps) {
				if (map.contains(x, y)) {
					return map.map(x, y);
				}
			}

			auto min_index = -1;
			double min_distance = std::numeric_limits<double>::infinity();
			for (auto i = 0; i < (int) maps.size(); i++) {
				const auto md = maps[i].distance_to_edge(x, y);
				if (!md.is_valid()) {
					std::cerr << "Unable to calculate distance to triangle." << std::endl;
				}
				const auto d = md.get_or(std::numeric_limits<double>::infinity());
				if (d < min_distance) {
					min_distance = d;
					min_index = i;
				}
			}
			if (min_index < 0) {
				// TODO:
				throw std::runtime_error{"No closest triangle found."};
				return geometry::Point{0, 0};
			}
			return maps[min_index].map(x, y);
		}

		[[nodiscard]] inline
		Point map(const Point& p) const {
			return map(p.x, p.y);
		}
	};

}

#endif //IDEA_MAP_H
