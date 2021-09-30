//
// Created by thallock on 9/21/21.
//

#ifndef IDEA_TEXT_H
#define IDEA_TEXT_H


#include "./GraphicsPrimitive.h"

#include "../geometry/Point.h"
#include "Color.h"

namespace billiards::graphics {

	class Text : public GraphicsPrimitive {
	public:
		Color color;
		geometry::Point location;
		std::string text;
		std::string font_family;
		double font_size;

		Text()
			: color{1, 1, 1, 1}
			, text{}
			, font_family{"DejaVu Sans Mono"}
			, font_size{16}
			{}

		~Text() override = default;


		[[nodiscard]] std::string get_type() const override { return "text"; };

		void parse(const nlohmann::json& value) override {
			if (value.contains("color") && value["color"].is_object()) {
				color.parse(value["color"]);
			}
			if (value.contains("location") && value["location"].is_object()) {
				location.parse(value["location"]);
			}
			if (value.contains("text") && value["text"].is_string()) {
				text = value["text"].get<std::string>();
			}
			if (value.contains("font-family") && value["font-family"].is_string()) {
				font_family = value["font-family"].get<std::string>();
			}
			if (value.contains("font-size") && value.is_number()) {
				font_size = value["font-size"].get<double>();
			}
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			GraphicsPrimitive::to_json(writer);
			writer.key("color");
			color.to_json(writer);
			writer.key("location");
			location.to_json(writer);
			writer.field("text", text);
			writer.field("font-family", font_family);
			writer.field("font-size", font_size);
			writer.end_object();
		}
	};

}


#endif //IDEA_TEXT_H
