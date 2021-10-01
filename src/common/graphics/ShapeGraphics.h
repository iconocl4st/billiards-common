//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_SHAPEGRAPHICS_H
#define GLVIEW_SHAPEGRAPHICS_H

#include "./GraphicsPrimitive.h"

#include "Color.h"

namespace billiards::graphics {

	class ShapeGraphics : public GraphicsPrimitive {
	public:
		Color color;
		bool fill;
		double line_width;

		ShapeGraphics() : color{255, 255, 255, 255}, fill{true}, line_width{-1} {}
//		ShapeGraphics(Color color, bool fill) : color{color}, fill{}

		~ShapeGraphics() override = default;

		void parse(const nlohmann::json& value, json::ParseResult& result) override {
			ENSURE_OBJECT(result, value, "color", "shapes must have a color");
			PARSE_CHILD(result, value["color"], color);

			const bool has_fill = HAS_BOOL(value, "fill");
			const bool has_width = HAS_BOOL(value, "line-width");
			if (!has_fill && !has_width) {
				result.success = false;
				result.error_msg << "shapes must either have a fill or a line width";
				return;
			}
			if (has_fill) {
				fill = value["fill"].get<bool>();
			}
			if (has_width) {
				line_width = value["line-width"].get<double>();
				fill = false;
			}
		}

		void to_json(json::SaxWriter& writer) const override {
			GraphicsPrimitive::to_json(writer);

			writer.key("color");
			color.to_json(writer);

			if (fill) {
				writer.field("fill", true);
			} else {
				writer.field("line-width", line_width);
			}
		}
	};

}

#endif //GLVIEW_SHAPEGRAPHICS_H
