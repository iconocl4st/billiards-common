//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_TABLE_H
#define IDEA_TABLE_H

#include "./Ball.h"
#include "./Pocket.h"
#include "./Rail.h"

namespace billiards::config {
	class Table : public json::Serializable {
	public:
		std::array<BallInfo, 16> balls;
		std::array<Pocket, 6> pockets;
		std::array<Rail, 6> rails;

		Table() :
			balls{
			BallInfo{   CUE, 2.26 / 2, gphx::color::from_int(255, 255, 255),  0, "cue"},
			BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(255, 245,  64),  1, "one"},
			BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int( 43,  59, 179),  2, "two"},
			BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(255,   0,   0),  3, "three"},
			BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int( 26,   1,  94),  4, "four"},
			BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(255, 159,  41),  5, "five"},
			BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(  9, 148,  30),  6, "six"},
			BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(255,  25,  98),  7, "seven"},
			BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(  0,   0,   0),  8, "eight"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(255, 245,  64),  9, "nine"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int( 43,  59, 179), 10, "ten"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(255,   0,   0), 11, "eleven"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int( 26,   1,  94), 12, "twelve"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(255, 159,  41), 13, "thirteen"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(  9, 148,  30), 14, "fourteen"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(255,  25,  98), 15, "fifteen"},
			},
			pockets{
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
					geometry::Point{93.66969147, 43.33333333}}
			},
			rails{}
		{

		}

		virtual ~Table() = default;

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
			writer.end_object();
		};

		void parse(const nlohmann::json& value) {
			if (value.contains("balls") && value["balls"].is_array()
					&& value["balls"].size() == balls.size()) {
				auto length = balls.size();
				for (int i=0; i<length; i++) {
					balls[i].parse(value["balls"][i]);
				}
			}
			if (value.contains("pockets") && value["pockets"].is_array()
					&& value["pockets"].size() == pockets.size()) {
				auto length = pockets.size();
				for (int i=0; i<length; i++) {
					pockets[i].parse(value["pockets"][i]);
				}
			}
		};
	};
}
#endif //IDEA_TABLE_H
