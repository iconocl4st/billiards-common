//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_LINES_H
#define GLVIEW_LINES_H


#include <cassert>
#include <list>

#include "ShapeGraphics.h"
#include "../geometry/Point.h"

namespace billiards::gphx {

	class Lines : public ShapeGraphics {
	public:
		std::list<geometry::Point> points;

		Lines() = default;
		~Lines() override = default;

		void parse(const nlohmann::json& value) override {
			ShapeGraphics::parse(value);

			if (value.contains("points") && value["points"].is_array()) {
				for (const auto & itr : value["points"]) {
					geometry::Point p;
					p.parse(itr);
					points.push_back(p);
				}
			}
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			ShapeGraphics::to_json(writer);

			writer.key("points");
			writer.begin_array();
			for (const auto & point : points) {
				point.to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}

		[[nodiscard]] std::string get_type() const override { return "line"; }
	};
}


#endif //GLVIEW_LINES_H
