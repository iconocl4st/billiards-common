//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_COLOR_H
#define GLVIEW_COLOR_H

#include <cassert>

#include "../utils/Serializable.h"

namespace billiards::graphics {

	class Color : public json::Serializable {
	public:
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;

		explicit Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
			: red{red}
			, green{green}
			, blue{blue}
			, alpha{alpha}
		{}

//		explicit Color(uint8_t red, uint8_t green, uint8_t blue)
//			: Color{red, green, blue, 255}
//		{}
//
		Color() : Color{0, 0, 0, 0} {}
//
//		Color(double red, double green, double blue, double alpha)
//			: Color{(uint8_t) (255 * red)
//			, (uint8_t) (255 * green)
//			, (uint8_t) (255 * blue)
//			, (uint8_t) (255 * alpha)}
//		{
//		}
//
//		Color(double red, double green, double blue) : Color{red, green, blue, 1} {}


		void parse(const nlohmann::json& value) override {
			if (value.contains("r") && value["r"].is_number()) {
				red = value["r"].get<double>();
			}
			if (value.contains("g") && value["g"].is_number()) {
				green = value["g"].get<double>();
			}
			if (value.contains("b") && value["b"].is_number()) {
				blue = value["b"].get<double>();
			}
			if (value.contains("a") && value["a"].is_number()) {
				alpha = value["a"].get<double>();
			}
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("r", red);
			writer.field("g", green);
			writer.field("b", blue);
			writer.field("a", alpha);
			writer.end_object();
		}
	};

	// multiply defined?
//	const Color RED{255, 0, 0};
//	const Color GREEN{0, 255, 0};
//	const Color BLUE{0, 0, 255};
//	const Color WHITE{255, 255, 255};
//	const Color BLACK{0, 0, 0};

	namespace color {
		inline
		Color from_int(int r, int g, int b, int a) {
			assert(r >= 0 && r <= 255);
			assert(g >= 0 && g <= 255);
			assert(b >= 0 && b <= 255);
			assert(a >= 0 && a <= 255);
			return Color{(uint8_t) r, (uint8_t) g, (uint8_t) b, (uint8_t) a};
		}
		inline
		Color from_int(int r, int g, int b) {
			return from_int(r, g, b, 255);
		}

		inline
		Color from_double(double r, double g, double b, double a) {
			assert(r >= 0 && r <= 1.0);
			assert(g >= 0 && g <= 1.0);
			assert(b >= 0 && b <= 1.0);
			assert(a >= 0 && a <= 1.0);
			return Color{
				(uint8_t) (255 * r),
				(uint8_t) (255 * g),
				(uint8_t) (255 * b),
				(uint8_t) (255 * a)};
		}

		inline
		Color from_double(double r, double g, double b) {
			return from_double(r, g, b, 1.0);
		}
	}
}


#endif //GLVIEW_COLOR_H
