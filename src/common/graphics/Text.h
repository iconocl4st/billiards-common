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
			: color{255, 255, 255, 255}
			, text{}
			, font_family{"DejaVu Sans Mono"}
			, font_size{16}
			{}

		~Text() override = default;


		[[nodiscard]] std::string get_type() const override { return "text"; };

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			if (HAS_OBJECT(value, "color")) {
				PARSE_CHILD(status, value["color"], color);
			}
			if (HAS_STRING(value, "text")) {
				text = value["text"].get<std::string>();
			}
			if (HAS_STRING(value, "font-family")) {
				font_family = value["font-family"].get<std::string>();
			}
			if (HAS_NUMBER(value, "font-size")) {
				font_size = value["font-size"].get<double>();
			}
			REQUIRE_CHILD(status, value, "location", location, "Text must be located");
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
