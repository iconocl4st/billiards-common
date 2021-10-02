//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_BALLDIMENSIONS_H
#define GLVIEW_BALLDIMENSIONS_H

#include <array>

#include "billiards_common/graphics/Color.h"

namespace billiards::config {

	namespace ball_type {
		enum BallType {
			SOLID,
			STRIPE,
			CUE,
			UNKNOWN,
		};

		[[nodiscard]] inline
		std::string to_string(BallType type) {
			switch (type) {
				case SOLID:
					return "solid";
				case STRIPE:
					return "stripe";
				case CUE:
					return "cue";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}

		[[nodiscard]] inline
		BallType from_string(const std::string& str) {
			if (str == "solid") {
				return SOLID;
			} else if (str == "stripe") {
				return STRIPE;
			} else if (str == "cue") {
				return CUE;
			} else {
				return UNKNOWN;
			}
		}
	}


	class BallInfo : json::Serializable {
	public:
		ball_type::BallType ball_type;
		double radius;
		graphics::Color color;
		int number;
		std::string name;
		
		BallInfo(ball_type::BallType ball_type, double radius, graphics::Color color, int number, const char *name) :
			ball_type{ball_type},
			radius{radius},
			color{color},
			number{number},
			name{name} {}

		BallInfo() : BallInfo{ball_type::UNKNOWN, -1, graphics::Color{0, 0, 0, 0}, -1, "uninitialized"} {}

		~BallInfo() = default;

		[[nodiscard]] inline
		bool is_cue() const { return ball_type == ball_type::CUE; }

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
			writer.field("type", ball_type::to_string(ball_type));
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_NUMBER(status, value, "radius", "Ball info must have a radius");
			radius = value["radius"].get<double>();
			ENSURE_NUMBER(status, value, "number", "Ball info must have a number");
			number = value["number"].get<int>();
			ENSURE_STRING(status, value, "name", "Ball info must have a name");
			name = value["name"].get<std::string>();
			ENSURE_STRING(status, value, "type", "Ball info must have a type");
			ball_type = ball_type::from_string(value["type"].get<std::string>());
			REQUIRE_CHILD(status, value, "color", color, "Ball info must have a color");
		}
	};

	namespace balls {
		inline
		std::array<BallInfo, 16> ALL_BALLS(){ return std::array<BallInfo, 16>{
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
		}; }
	}
}

#endif //GLVIEW_BALLDIMENSIONS_H
