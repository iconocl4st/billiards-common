//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_TABLE_H
#define IDEA_TABLE_H

#include "billiards_common/geometry/Dimensions.h"
#include "billiards_common/geometry/geometry.h"

#include "Ball.h"
#include "Pocket.h"
#include "Rail.h"

#define POCKET_DEF(X, Y, THETA) Pocket{ \
	geometry::Point{X + 3.0 * std::cos(THETA + M_PI / 4), Y + 3.0 * std::sin(THETA + M_PI / 4)},	\
	geometry::Point{X + 0.5 * std::cos(THETA + M_PI / 4), Y + 0.5 * std::sin(THETA + M_PI / 4)},	\
	geometry::Point{X + 3.0 * std::cos(THETA - M_PI / 4), Y + 3.0 * std::sin(THETA - M_PI / 4)}}


namespace billiards::config {

	namespace constants {
		constexpr int NUM_RAILS = 6;

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

	class Table : public json::Serializable {
	public:


		std::array<BallInfo, 16> balls;
		std::array<Pocket, 6> pockets;
		geometry::Dimensions dims;

		Table()
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
			, pockets{
				POCKET_DEF(92, 0, 0.75 * M_PI),
				POCKET_DEF(51, -2, 0.5 * M_PI),
				POCKET_DEF(0, 0, 0.25 * M_PI),
				POCKET_DEF(0, 46, -0.25 * M_PI),
				POCKET_DEF(51, 48, -0.5 * M_PI),
				POCKET_DEF(92, 46, -0.75 * M_PI)
			}
			, dims{92, 46}
		{
			pockets[constants::RIGHT_LOWER_POCKET].orientation = PocketOrientation{horz_loc::RIGHT, vert_loc::LOWER};
			pockets[constants::MIDDLE_LOWER_POCKET].orientation = PocketOrientation{horz_loc::MIDDLE, vert_loc::LOWER};
			pockets[constants::LEFT_LOWER_POCKET].orientation = PocketOrientation{horz_loc::LEFT, vert_loc::LOWER};
			pockets[constants::RIGHT_UPPER_POCKET].orientation = PocketOrientation{horz_loc::LEFT, vert_loc::UPPER};
			pockets[constants::MIDDLE_UPPER_POCKET].orientation = PocketOrientation{horz_loc::MIDDLE, vert_loc::UPPER};
			pockets[constants::LEFT_UPPER_POCKET].orientation = PocketOrientation{horz_loc::RIGHT, vert_loc::UPPER};
		}

		virtual ~Table() = default;

		[[nodiscard]] inline
		Rail rail(const int p1, const int p2) const {
			const int pocket1 = std::min(p1, p2);
			const int pocket2 = std::max(p1, p2);
			if (pocket1 == pocket2) {
				throw std::runtime_error{"No rail between the same pocket..."};
			}
			const geometry::Point rail_center = (pockets[pocket1].outer_segment_1 + pockets[pocket2].outer_segment_2) / 2.0;
			const double d11 = (pockets[pocket1].outer_segment_1 - rail_center).norm2();
			const double d12 = (pockets[pocket1].outer_segment_2 - rail_center).norm2();
			const double d21 = (pockets[pocket2].outer_segment_1 - rail_center).norm2();
			const double d22 = (pockets[pocket2].outer_segment_2 - rail_center).norm2();
			const geometry::Point inner1 = d11 <= d12 ? pockets[pocket1].outer_segment_1 : pockets[pocket1].outer_segment_2;
			const geometry::Point inner2 = d21 <= d22 ? pockets[pocket2].outer_segment_1 : pockets[pocket2].outer_segment_2;
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

		[[nodiscard]] inline
		bool is_incident(int pocket_index, int rail_index) const {
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

		[[nodiscard]] inline
		const Pocket& get_pocket(int pocket_index) const {
			if (pocket_index < 0 || pocket_index >= pockets.size()) {
				throw std::runtime_error{"Invalid pocket"};
			}
			return pockets[pocket_index];
		}

		void to_json(json::SaxWriter& writer) const {
			writer.begin_object();
			writer.key("balls");
			writer.begin_array();
			for (const auto& ball : balls) {
				ball.to_json(writer);
			}
			writer.end_array();
			writer.key("pockets");
			writer.begin_array();
			for (const auto& pocket : pockets) {
				pocket.to_json(writer);
			}
			writer.end_array();
			writer.key("dimenions");
			dims.to_json(writer);
			writer.end_object();
		};

		void parse(const nlohmann::json& value, json::ParseResult& status) {
			ENSURE_ARRAY(status, value, "balls", "table must have balls");
			// value["balls"].size() == balls.size()
			auto num_balls = balls.size();
			for (int i = 0; i < num_balls; i++) {
				balls[i].parse(value["balls"][i], status);
				if (!status.success) {
					return;
				}
			}
			ENSURE_ARRAY(status, value, "pockets", "table must have pockets");
//			value["pockets"].size() == pockets.size()
			auto num_pockets = pockets.size();
			for (int i = 0; i < num_pockets; i++) {
				pockets[i].parse(value["pockets"][i], status);
				if (!status.success) {
					return;
				}
			}

			REQUIRE_CHILD(status, value, "dimensions", dims, "Table must have dimensions");
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
#endif //IDEA_TABLE_H
