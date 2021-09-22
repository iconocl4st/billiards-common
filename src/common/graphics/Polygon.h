//
// Created by thallock on 9/19/21.
//

#ifndef IDEA_POLYGON_H
#define IDEA_POLYGON_H

#include <list>

#include "ShapeGraphics.h"
#include "../geometry/Point.h"

namespace billiards::graphics {

	// For convex polygons...
	class Polygon : public ShapeGraphics {
	public:
		std::list<geometry::Point> vertices;

		Polygon() = default;
		~Polygon() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();

			ShapeGraphics::to_json(writer);

			writer.key("vertices");
			writer.begin_array();
			for (const auto& point : vertices) {
				point.to_json(writer);
			}
			writer.end_array();

			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			vertices.clear();
			if (value.contains("vertices") && value["vertices"].is_array()) {
				for (const auto& obj : value["vertices"]) {
					vertices.emplace_back();
					vertices.back().parse(obj);
				}
			}
			ShapeGraphics::parse(value);
		}

		[[nodiscard]] std::string get_type() const override { return "polygon"; }
	};
}

#endif //IDEA_POLYGON_H
