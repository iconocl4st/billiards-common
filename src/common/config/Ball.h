//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_BALLDIMENSIONS_H
#define GLVIEW_BALLDIMENSIONS_H

#include "../graphics/Color.h"

#include <array>

namespace billiards::config {

	enum BallType {
		SOLID,
		STRIPE,
		CUE,
		UNINITIALIZED,
	};

	std::string ball_type_to_string(BallType type) {
		switch (type) {
			case SOLID:
				return "solid";
			case STRIPE:
				return "stripe";
			case CUE:
				return "cue";
			case UNINITIALIZED:
			default:
				return "unknown";
		}
	}

	BallType string_to_ball_type(const std::string& str) {
		if (str == "solid") {
			return SOLID;
		} else if (str == "stripe") {
			return STRIPE;
		} else if (str == "cue") {
			return CUE;
		} else {
			return UNINITIALIZED;
		}
	}


	class BallInfo : json::Serializable {
	public:
		BallType ball_type;
		double radius;
		graphics::Color color;
		int number;
		std::string name;
		
		BallInfo(BallType ball_type, double radius, graphics::Color color, int number, const char *name) :
			ball_type{ball_type},
			radius{radius},
			color{color},
			number{number},
			name{name} {}

		BallInfo() : BallInfo{UNINITIALIZED, -1, graphics::Color{0, 0, 0, 0}, -1, "uninitialized"} {}

		~BallInfo() = default;

		bool is_cue() const { return ball_type == CUE; }

		bool operator==(const BallInfo& other) const {
			return number == other.number /* && ball_type == other.ball_type */;
		}
		
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("radius", radius);
			writer.field("number", number);
			writer.field("name", name);
			writer.key("color");
			color.to_json(writer);
			writer.field("ball-type", ball_type_to_string(ball_type));
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value) override {
			if (value.contains("radius") && value["radius"].is_number()) {
				radius = value["radius"].get<double>();
			}
			if (value.contains("number") && value["number"].is_number()) {
				number = value["number"].get<int>();
			}
			if (value.contains("color") && value["color"].is_object()) {
				color.parse(value["color"]);
			}
			if (value.contains("name") && value["name"].is_string()) {
				name = value["name"].get<std::string>();
			}
			if (value.contains("ball-type") && value["ball-type"].is_string()) {
				ball_type = string_to_ball_type(value["ball-type"].get<std::string>());
			}
		}

	};

	namespace balls {
		inline
		std::array<BallInfo, 16> ALL_BALLS(){ return std::array<BallInfo, 16>{
			 BallInfo{   CUE, 2.26 / 2, graphics::color::from_int(255, 255, 255),  0, "cue"},
			 BallInfo{ SOLID, 2.26 / 2, graphics::color::from_int(255, 245,  64),  1, "one"},
			 BallInfo{ SOLID, 2.26 / 2, graphics::color::from_int( 43,  59, 179),  2, "two"},
			 BallInfo{ SOLID, 2.26 / 2, graphics::color::from_int(255,   0,   0),  3, "three"},
			 BallInfo{ SOLID, 2.26 / 2, graphics::color::from_int( 26,   1,  94),  4, "four"},
			 BallInfo{ SOLID, 2.26 / 2, graphics::color::from_int(255, 159,  41),  5, "five"},
			 BallInfo{ SOLID, 2.26 / 2, graphics::color::from_int(  9, 148,  30),  6, "six"},
			 BallInfo{ SOLID, 2.26 / 2, graphics::color::from_int(255,  25,  98),  7, "seven"},
			 BallInfo{ SOLID, 2.26 / 2, graphics::color::from_int(  0,   0,   0),  8, "eight"},
			 BallInfo{STRIPE, 2.26 / 2, graphics::color::from_int(255, 245,  64),  9, "nine"},
			 BallInfo{STRIPE, 2.26 / 2, graphics::color::from_int( 43,  59, 179), 10, "ten"},
			 BallInfo{STRIPE, 2.26 / 2, graphics::color::from_int(255,   0,   0), 11, "eleven"},
			 BallInfo{STRIPE, 2.26 / 2, graphics::color::from_int( 26,   1,  94), 12, "twelve"},
			 BallInfo{STRIPE, 2.26 / 2, graphics::color::from_int(255, 159,  41), 13, "thirteen"},
			 BallInfo{STRIPE, 2.26 / 2, graphics::color::from_int(  9, 148,  30), 14, "fourteen"},
			 BallInfo{STRIPE, 2.26 / 2, graphics::color::from_int(255,  25,  98), 15, "fifteen"},
		}; }
	}
}

#endif //GLVIEW_BALLDIMENSIONS_H
