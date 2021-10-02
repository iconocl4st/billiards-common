//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_CIRCLE_H
#define GLVIEW_CIRCLE_H

#include "billiards_common/graphics/ShapeGraphics.h"

#include "billiards_common/geometry/Point.h"

namespace billiards::graphics {

	class Circle : public ShapeGraphics {
	public:
		geometry::Point center;
		double radius;

		Circle(
			const geometry::Point &center,
			double radius
		): center{ center }, radius{ radius } {}

		Circle() : Circle{geometry::Point{}, 0.0} {}
		~Circle() override = default;

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			REQUIRE_CHILD(status, value, "center", center, "circles must have a center");
			ENSURE_NUMBER(status, value, "r", "circles must have a radius");
			radius = value["r"].get<double>();
			ShapeGraphics::parse(value, status);
		}
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();

			ShapeGraphics::to_json(writer);

			writer.key("center");
			center.to_json(writer);
			writer.field("r", radius);

			writer.end_object();
		}

		[[nodiscard]] std::string get_type() const override { return "circle"; }
	};
}

#endif //GLVIEW_CIRCLE_H
