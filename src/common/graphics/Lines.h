//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_LINES_H
#define GLVIEW_LINES_H


#include <cassert>
#include <list>

#include "ShapeGraphics.h"
#include "../geometry/Point.h"

namespace billiards::graphics {

	class Lines : public ShapeGraphics {
	public:
		std::list<std::pair<geometry::Point, geometry::Point>> segments;

		Lines() = default;
		~Lines() override = default;

		inline
		void append(double x1, double y1, double x2, double y2) {
			segments.emplace_back(
				std::pair{geometry::Point{x1, y1}, geometry::Point{x2, y2}});
		}

		inline
		void append(const geometry::Point& p1, const geometry::Point& p2) {
			segments.emplace_back(std::pair{p1, p2});
		}

		void parse(const nlohmann::json& value) override {
			ShapeGraphics::parse(value);

			if (!value.contains("segments") || !value["segments"].is_array()) {
				throw std::runtime_error{"lines must have segments"};
			}

			segments.clear();
			for (const auto& segment: value["segments"]) {
				geometry::Point begin;
				geometry::Point end;
				if (!segment.contains("begin") || !segment["begin"].is_object()) {
					throw std::runtime_error{"segments must have a begin"};
				}
				begin.parse(segment["begin"]);
				if (!segment.contains("end") || !segment["end"].is_object()) {
					throw std::runtime_error{"segments must have an end"};
				}
				end.parse(segment["end"]);
				segments.emplace_back(std::pair{begin, end});
			}
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			ShapeGraphics::to_json(writer);

			writer.key("segments");
			writer.begin_array();
			for (const auto & segment : segments) {
				writer.begin_object();
				writer.key("begin");
				segment.first.to_json(writer);
				writer.key("end");
				segment.second.to_json(writer);
				writer.end_object();
			}
			writer.end_array();
			writer.end_object();
		}

		[[nodiscard]] std::string get_type() const override { return "lines"; }
	};
}


#endif //GLVIEW_LINES_H
