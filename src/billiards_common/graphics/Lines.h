//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_LINES_H
#define GLVIEW_LINES_H

#include <cassert>
#include <list>

#include "billiards_common/geometry/Point.h"
#include "billiards_common/graphics/ShapeGraphics.h"

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

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_ARRAY(status, value, "segments", "lines must have segments");

			segments.clear();
			for (const auto& segment: value["segments"]) {
				ENSURE_OBJECT(status, segment, "begin", "segments must have a begin");
				geometry::Point begin;
				PARSE_CHILD(status, segment["begin"], begin);
				ENSURE_OBJECT(status, segment, "end", "segments must have an end");
				geometry::Point end;
				PARSE_CHILD(status, segment["end"], end);
				segments.push_back(std::pair{begin, end});
			}
			ShapeGraphics::parse(value, status);
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
