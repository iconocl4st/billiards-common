//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_PARSE_GRAPHICS_H
#define GLVIEW_PARSE_GRAPHICS_H

#include "Circle.h"
#include "Lines.h"
#include "Polygon.h"

namespace billiards::graphics {
	inline
	std::shared_ptr<graphics::GraphicsPrimitive> parse_graphics(const nlohmann::json& value, json::ParseResult& result) {
		GraphicsPrimitive *primitive = nullptr;
		if (!value.contains("type") || !value["type"].is_string()) {
			return std::shared_ptr<GraphicsPrimitive>{primitive};
		}

		std::string type = value["type"].get<std::string>();
		if (type == "circle") {
			primitive = new Circle;
		} else if (type == "lines") {
			primitive = new Lines;
		} else if (type == "polygon") {
			primitive = new Polygon;
		} else {
			return std::shared_ptr<GraphicsPrimitive>{primitive};
		}

		primitive->parse(value, result);

		return std::shared_ptr<GraphicsPrimitive>{primitive};
	}
}

#endif //GLVIEW_PARSE_GRAPHICS_H
