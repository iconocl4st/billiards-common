//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_SHAPEGRAPHICS_H
#define GLVIEW_SHAPEGRAPHICS_H

#include "./GraphicsPrimitive.h"

#include "Color.h"

namespace billiards::gphx {

	class ShapeGraphics : public GraphicsPrimitive {
	public:
		Color color;
		bool fill;
		double line_width;

		ShapeGraphics() : color{1, 1, 1}, fill{true}, line_width{0} {}
//		ShapeGraphics(Color color, bool fill) : color{color}, fill{}

		~ShapeGraphics() override = default;

		void parse(const nlohmann::json& value) override {
			if (value.contains("color") && value.is_object()) {
				color.parse(value["color"]);
			}
			if (value.contains("fill") && value.is_boolean()) {
				fill = value["fill"].get<bool>();
			}
			if (value.contains("line-width") && value.is_number()) {
				line_width = value["line-width"].get<double>();
			}
		}

		void to_json(json::SaxWriter& writer) const override {
			GraphicsPrimitive::to_json(writer);

			writer.key("color");
			color.to_json(writer);
			writer.field("fill", fill);
			writer.field("line-width", line_width);
		}
	};

}

#endif //GLVIEW_SHAPEGRAPHICS_H
