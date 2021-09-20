//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_COLOR_H
#define GLVIEW_COLOR_H

#include <cassert>

#include "../utils/Serializable.h"

namespace billiards::gphx {

class Color : public json::Serializable {
	public:
		// TODO: Make these be uint8_t...
		double red;
		double green;
		double blue;
		double alpha;

		Color(double red, double green, double blue, double alpha) : red{red}, green{green}, blue{blue}, alpha{alpha} {
			assert(red >= 0 && red <= 1.0);
			assert(green >= 0 && green <= 1.0);
			assert(blue >= 0 && blue <= 1.0);
			assert(alpha >= 0 && alpha <= 1.0);
		}
		
		Color(double red, double green, double blue) : Color{red, green, blue, 1} {}
		
		Color() : Color{0, 0, 0} {}

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
	const Color RED{1.0, 0.0, 0.0};
	const Color GREEN{0.0, 1.0, 0.0};
	const Color BLUE{0.0, 0.0, 1.0};
	const Color WHITE{1.0, 1.0, 1.0};
	const Color BLACK{1.0, 1.0, 1.0};

	namespace color {
		inline
		Color from_int(int r, int g, int b) { return Color{r / 255.0, g / 255.0, b / 255.0}; }
	}
}


#endif //GLVIEW_COLOR_H
