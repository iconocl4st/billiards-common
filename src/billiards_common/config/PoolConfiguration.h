//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_POOLCONFIGURATION_H
#define IDEA_POOLCONFIGURATION_H

#include "billiards_common/config/TableDimensions.h"
#include "billiards_common/config/TableGeometry.h"
#include "billiards_common/geometry/geometry.h"

#include "billiards_common/config/Ball.h"
#include "billiards_common/config/Pocket.h"
#include "billiards_common/config/Rail.h"


/*
 * There should be some sort of default, or ideal pool config determined only by a table dimensions.
 * and a default ball radius?
 */


namespace billiards::config {

	namespace constants {
		constexpr int NUM_RAILS = 6;
		constexpr int NUM_POCKETS = 6;

		constexpr int RIGHT_LOWER_POCKET = 0;
		constexpr int MIDDLE_LOWER_POCKET = 1;
		constexpr int LEFT_LOWER_POCKET = 2;
		constexpr int LEFT_UPPER_POCKET = 3;
		constexpr int MIDDLE_UPPER_POCKET = 4;
		constexpr int RIGHT_UPPER_POCKET = 5;


		constexpr int RIGHT_RAIL = 0;
		constexpr int UPPER_RIGHT_RAIL = 1;
		constexpr int UPPER_LEFT_RAIL = 2;
		constexpr int LEFT_RAIL = 3;
		constexpr int LOWER_LEFT_RAIL = 4;
		constexpr int LOWER_RIGHT_RAIL = 5;
	}

    // This should be renamed, it has more info than just the table: it has the ball colors...
	class PoolConfiguration : public json::Serializable {
	public:
		std::array<BallInfo, 16> balls;
		// Should be generated from the geometry...
//		std::array<Pocket, 6> pockets;
		TableDimensions dims;
		// TODO: Should be configurable...
//		TableGeometry geometry;


		// Should be able to create a default Config from a TableDimensions....
		PoolConfiguration()
			: balls{
			BallInfo{   ball_type::CUE, 2.26 / 2, graphics::color::from_int(255, 255, 255),  0, "cue"},
			BallInfo{ ball_type::SOLID, 2.26 / 2, graphics::color::from_int(255, 245,  64),  1, "one"},
			BallInfo{ ball_type::SOLID, 2.26 / 2, graphics::color::from_int( 43,  59, 179),  2, "two"},
			BallInfo{ ball_type::SOLID, 2.26 / 2, graphics::color::from_int(255,   0,   0),  3, "three"},
			BallInfo{ ball_type::SOLID, 2.26 / 2, graphics::color::from_int( 26,   1,  94),  4, "four"},
			BallInfo{ ball_type::SOLID, 2.26 / 2, graphics::color::from_int(255, 159,  41),  5, "five"},
			BallInfo{ ball_type::SOLID, 2.26 / 2, graphics::color::from_int(  9, 148,  30),  6, "six"},
			BallInfo{ ball_type::SOLID, 2.26 / 2, graphics::color::from_int(255,  25,  98),  7, "seven"},
			BallInfo{ ball_type::SOLID, 2.26 / 2, graphics::color::from_int(  0,   0,   0),  8, "eight"},
			BallInfo{ball_type::STRIPE, 2.26 / 2, graphics::color::from_int(255, 245,  64),  9, "nine"},
			BallInfo{ball_type::STRIPE, 2.26 / 2, graphics::color::from_int( 43,  59, 179), 10, "ten"},
			BallInfo{ball_type::STRIPE, 2.26 / 2, graphics::color::from_int(255,   0,   0), 11, "eleven"},
			BallInfo{ball_type::STRIPE, 2.26 / 2, graphics::color::from_int( 26,   1,  94), 12, "twelve"},
			BallInfo{ball_type::STRIPE, 2.26 / 2, graphics::color::from_int(255, 159,  41), 13, "thirteen"},
			BallInfo{ball_type::STRIPE, 2.26 / 2, graphics::color::from_int(  9, 148,  30), 14, "fourteen"},
			BallInfo{ball_type::STRIPE, 2.26 / 2, graphics::color::from_int(255,  25,  98), 15, "fifteen"},
			}
			, dims{}
//			, geometry{dims}
		{

		}

		~PoolConfiguration() override = default;

		[[nodiscard]] inline
		Rail rail(const int p1, const int p2) const {
			const auto pckts = pockets();
			const int pocket1 = std::min(p1, p2);
			const int pocket2 = std::max(p1, p2);
			if (pocket1 == pocket2) {
				throw std::runtime_error{"No rail between the same pocket..."};
			}
			const geometry::Point rail_center = (pckts[pocket1].outer_segment_1 + pckts[pocket2].outer_segment_2) / 2.0;
			const double d11 = (pckts[pocket1].outer_segment_1 - rail_center).norm2();
			const double d12 = (pckts[pocket1].outer_segment_2 - rail_center).norm2();
			const double d21 = (pckts[pocket2].outer_segment_1 - rail_center).norm2();
			const double d22 = (pckts[pocket2].outer_segment_2 - rail_center).norm2();
			const geometry::Point inner1 = d11 <= d12 ? pckts[pocket1].outer_segment_1 : pckts[pocket1].outer_segment_2;
			const geometry::Point inner2 = d21 <= d22 ? pckts[pocket2].outer_segment_1 : pckts[pocket2].outer_segment_2;
			const geometry::MaybeLine rail_line = geometry::through(inner1, inner2);
			const geometry::MaybePoint table_center = geometry::MaybePoint{dims.width / 2, dims.height / 2};
			const geometry::MaybeLine orthogonal = geometry::orthogonal_at(rail_line, table_center);
			const geometry::MaybePoint inter = geometry::intersection(rail_line, orthogonal);
			const geometry::Point direction = (table_center - inter).get();
			const double norm = direction.norm();
			if (norm < TOLERANCE) {
				throw std::runtime_error{"zero length direction"};
			}
			return Rail(inner1, inner2, direction / norm);
		}

		[[nodiscard]] inline
		Rail rail(int rail_index) const {
			switch (rail_index) {
				case constants::RIGHT_RAIL: return rail(constants::RIGHT_LOWER_POCKET, constants::RIGHT_UPPER_POCKET);
				case constants::UPPER_RIGHT_RAIL: return rail(constants::RIGHT_UPPER_POCKET, constants::MIDDLE_UPPER_POCKET);
				case constants::UPPER_LEFT_RAIL: return rail(constants::MIDDLE_UPPER_POCKET, constants::LEFT_UPPER_POCKET);
				case constants::LEFT_RAIL: return rail(constants::LEFT_UPPER_POCKET, constants::LEFT_LOWER_POCKET);
				case constants::LOWER_LEFT_RAIL: return rail(constants::LEFT_LOWER_POCKET, constants::MIDDLE_LOWER_POCKET);
				case constants::LOWER_RIGHT_RAIL: return rail(constants::MIDDLE_LOWER_POCKET, constants::RIGHT_LOWER_POCKET);
				default:
					throw std::runtime_error{"Invalid rail index"};
			}
		}

		[[nodiscard]] static inline
		bool is_incident(int pocket_index, int rail_index) {
			switch (pocket_index) {
				case constants::RIGHT_LOWER_POCKET:
					return rail_index == constants::RIGHT_RAIL
						   || rail_index == constants::LOWER_RIGHT_RAIL;
				case constants::MIDDLE_LOWER_POCKET:
					return rail_index == constants::LOWER_RIGHT_RAIL
						   || rail_index == constants::LOWER_LEFT_RAIL;
				case constants::LEFT_LOWER_POCKET:
					return rail_index == constants::LOWER_LEFT_RAIL
						   || rail_index == constants::LEFT_RAIL;
				case constants::LEFT_UPPER_POCKET:
					return rail_index == constants::LEFT_RAIL
						   || rail_index == constants::UPPER_LEFT_RAIL;
				case constants::MIDDLE_UPPER_POCKET:
					return rail_index == constants::UPPER_LEFT_RAIL
						   || rail_index == constants::UPPER_RIGHT_RAIL;
				case constants::RIGHT_UPPER_POCKET:
					return rail_index == constants::UPPER_RIGHT_RAIL
						   || rail_index == constants::RIGHT_RAIL;
				default:
					throw std::runtime_error{"Invalid pocket index"};
			}
		}
		/*
	 0		1	2		3
 4							5

 6							7
	 8		9	10		11
 */

		const std::array<Pocket, constants::NUM_POCKETS> pockets() const {
			config::TableGeometry g{dims};
			const geometry::Point center = geometry::Point{dims.width / 2.0, dims.height / 2.0};
			return std::array<Pocket, constants::NUM_POCKETS> {
				Pocket{g.point(11), g.point(11) * 1.1 - center * 0.1, g.point( 7), PocketOrientation{horz_loc::RIGHT, vert_loc::LOWER}},
				Pocket{g.point( 9), g.point( 9) * 1.1 - center * 0.1, g.point(10), PocketOrientation{horz_loc::MIDDLE, vert_loc::LOWER}},
				Pocket{g.point( 6), g.point( 6) * 1.1 - center * 0.1, g.point( 8), PocketOrientation{horz_loc::LEFT, vert_loc::LOWER}},
				Pocket{g.point( 4), g.point( 4) * 1.1 - center * 0.1, g.point( 0), PocketOrientation{horz_loc::LEFT, vert_loc::UPPER}},
				Pocket{g.point( 1), g.point( 1) * 1.1 - center * 0.1, g.point( 2), PocketOrientation{horz_loc::MIDDLE, vert_loc::UPPER}},
				Pocket{g.point( 3), g.point( 3) * 1.1 - center * 0.1, g.point( 5), PocketOrientation{horz_loc::RIGHT, vert_loc::UPPER}},
			};
		}

//		[[nodiscard]] inline
//		const Pocket& get_pocket(int pocket_index) const {
//			const auto pkts = pockets();
//			if (pocket_index < 0 || pocket_index >= pkts.size()) {
//				throw std::runtime_error{"Invalid pocket"};
//			}
//			return pkts[pocket_index];
//		}

		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
//			writer.field("ball-radius", 1.13);
			writer.key("balls");
			writer.begin_array();
			for (const auto& ball : balls) {
				ball.to_json(writer);
			}
			writer.end_array();


			// Maybe do away with this one?
			writer.key("pockets");
			writer.begin_array();
			const auto pkts = pockets();
			for (const auto& pocket : pkts) {
				pocket.to_json(writer);
			}
			writer.end_array();

			writer.key("dimensions");
			dims.to_json(writer);

			writer.key("physical-geometry");
			config::TableGeometry canonical{dims.width, dims.height, dims.pocket_width};
			canonical.to_json(writer);

			writer.end_object();
		};

		inline
		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			// TODO: parse a default ball radius...

			REQUIRE_CHILD(status, value, "dimensions", dims, "PoolConfiguration must have dimensions");
//			REQUIRE_CHILD(status, value, "geometry", geometry, "Table must have the geometry");

			ENSURE_ARRAY(status, value, "balls", "table must have balls");
			// value["balls"].size() == balls.size()
			auto num_balls = balls.size();
			for (int i = 0; i < num_balls; i++) {
				balls[i].parse(value["balls"][i], status);
				if (!status.success) {
					return;
				}
			}

//			ENSURE_ARRAY(status, value, "pockets", "table must have pockets");
////			value["pockets"].size() == pockets.size()
//			for (int i = 0; i < constants::NUM_POCKETS; i++) {
//				pockets[i].parse(value["pockets"][i], status);
//				if (!status.success) {
//					return;
//				}
//			}
		};
	};
}



/*
 *
 * Attempt:
				Pocket{
					geometry::Point{90.26352087,  2.86666667},
					geometry::Point{92.03339383,  1.66666667},
					geometry::Point{93.60290381,  5.83333333}},
				Pocket{
					geometry::Point{50.72522686,  3.06666667},
					geometry::Point{50.0907441 ,  1.66666667},
					geometry::Point{44.81451906,  3.03333333}},
				Pocket{
					geometry::Point{ 5.24283122,  2.93333333},
					geometry::Point{ 3.20580762,  1.66666667},
					geometry::Point{ 1.66969147,  5.93333333}},
				Pocket{
					geometry::Point{ 5.40980036, 46.86666667},
					geometry::Point{ 4.40798548, 47.66666667},
					geometry::Point{ 1.66969147, 43.66666667}},
				Pocket{
					geometry::Point{45.08166969, 46.96666667},
					geometry::Point{45.18185118, 47.66666667},
					geometry::Point{50.69183303, 46.93333333}},
				Pocket{
					geometry::Point{90.16333938, 46.73333333},
					geometry::Point{91.76624319, 47.66666667},
					geometry::Point{93.66969147, 43.33333333}
 */
#endif //IDEA_POOLCONFIGURATION_H
