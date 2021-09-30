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

		void parse(const nlohmann::json& value) override {
			if (!value.contains("color") || !value["color"].is_object()) {
				throw std::runtime_error{"shapes must have a color"};
			}
			color.parse(value["color"]);

			bool has_fill = value.contains("fill") && value["fill"].is_boolean();
			bool has_width = value.contains("line-width") && value["line-width"].is_number();
			if (!has_fill && !has_width) {
				throw std::runtime_error{"shapes must either have a fill or a line width"};
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
