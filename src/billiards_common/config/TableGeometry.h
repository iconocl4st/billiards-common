//
// Created by thallock on 12/22/21.
//

#ifndef IDEA_TABLEGEOMETRY_H
#define IDEA_TABLEGEOMETRY_H


#include "billiards_common/config/TableDimensions.h"


namespace billiards::config {

	/*
	 	0		1	2		3
	 4							5

	 6							7
	 	8		9	10		11
	 */

#define TABLE_GEOM_NUM_POINTS 12


//	constexpr double CANON_POCKET_WIDTH = 0.1;
//	constexpr double PD1 = CANON_POCKET_WIDTH;
//	constexpr double PD2 = CANON_POCKET_WIDTH / std::sqrt(2.0);

	class TableGeometry : public json::Serializable {
	public:
		double points[TABLE_GEOM_NUM_POINTS][2];

		TableGeometry() : points{} {}

		TableGeometry(double w, double h, double pw)
			: points{
				/* 00 */ {pw / std::sqrt(2), h},
				/* 01 */ {0.5 * (w - pw), h},
				/* 02 */ {0.5 * (w + pw), h},
				/* 03 */ {w - pw / std::sqrt(2), h},
				/* 04 */ {0, h - pw / std::sqrt(2)},
				/* 05 */ {w, h - pw / std::sqrt(2)},
				/* 06 */ {0, pw / std::sqrt(2)},
				/* 07 */ {w, pw / std::sqrt(2)},
				/* 08 */ {pw / std::sqrt(2), 0},
				/* 09 */ {0.5 * (w - pw), 0},
				/* 10 */ {0.5 * (w + pw), 0},
				/* 11 */ {w - pw / std::sqrt(2), 0},
			}
		{}

		TableGeometry(const TableDimensions& g)
			: TableGeometry{g.width, g.height, g.pocket_width}
		{}

		~TableGeometry() override = default;

		[[nodiscard]] inline
		geometry::Point lower_left() const {
			return geometry::Point{points[6][0], points[8][1]};
		}
		[[nodiscard]] inline
		geometry::Point lower_right() const {
			return geometry::Point{points[7][0], points[11][1]};
		}
		[[nodiscard]] inline
		geometry::Point upper_left() const {
			return geometry::Point{points[4][0], points[0][1]};
		}
		[[nodiscard]] inline
		geometry::Point upper_right() const {
			return geometry::Point{points[5][0], points[3][1]};
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("points");
			writer.begin_array();
			for (auto p : points) {
				writer.begin_array();
				for (auto j = 0; j < 2; j++) {
					writer.value(p[j]);
				}
				writer.end_array();
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_ARRAY(status, value, "points", "A geometry must have points.");

			const auto& array = value["points"];
			for (auto i = 0; i < TABLE_GEOM_NUM_POINTS; i++) {
				for (auto j = 0; j < 2; j++) {
					points[i][j] = array[i][j].get<double>();
					std::cout << points[i][j] << " ";
				}
				std::cout << '\n';
			}
			std::cout << std::endl;
		}
	};

}


#endif //IDEA_TABLEGEOMETRY_H
