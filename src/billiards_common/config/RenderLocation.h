//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_RENDERLOCATION_H
#define GLVIEW_RENDERLOCATION_H

#include "billiards_common/geometry/Point.h"
#include "billiards_common/geometry/Dimensions.h"

namespace billiards::project {

class RenderLocation : public json::Serializable {
public:
	geometry::Dimensions table_dims;

	geometry::Point of; // offset of table
	geometry::Point up; // straight up on table
	geometry::Point rt; // right on table

    RenderLocation()
		: table_dims{92, 46}
		, of{1900, -2}
		, up{17, 1977}
		, rt{-1875, 43}
    {}

	[[nodiscard]] inline
	geometry::Point map(const double x, const double y) const {
		double xn = x / table_dims.width;
		double yn = y / table_dims.height;
		return geometry::Point{
			of.x + rt.x * xn + up.x * yn,
			of.y + rt.y * xn + up.y * yn
		};
	}

	[[nodiscard]] inline
	geometry::Point map(const geometry::Point& in) const {
		return map(in.x, in.y);
	}

	[[nodiscard]] inline
	geometry::Point unmap(const double x, const double y) const {
		const double det = rt.x * up.y - up.x * rt.y;
		if (std::abs(det) < TOLERANCE) {
			return geometry::Point{};
		}
		const double xs = x - of.x;
		const double ys = y - of.y;
		const double xn = (+up.y * xs - up.x * ys) / det;
		const double yn = (-rt.y * xs + rt.x * ys) / det;

		return geometry::Point{xn * table_dims.width, yn * table_dims.height};
	}

	[[nodiscard]] inline
	geometry::Point unmap(const geometry::Point& in) const {
		return unmap(in.x, in.y);
	}

	void to_json(json::SaxWriter& writer) const override {
		writer.begin_object();
		writer.key("table-dims");
		table_dims.to_json(writer);
		writer.key("offset");
		of.to_json(writer);
		writer.key("up");
		up.to_json(writer);
		writer.key("right");
		rt.to_json(writer);
		writer.end_object();
	}

	void parse(const nlohmann::json& value, json::ParseResult& result) override {
		if (HAS_OBJECT(value, "table-dims")) {
			PARSE_CHILD(result, value["table-dims"], table_dims);
		}
		if (HAS_OBJECT(value, "offset")) {
			PARSE_CHILD(result, value["offset"], of);
		}
		if (HAS_OBJECT(value, "up")) {
			PARSE_CHILD(result, value["up"], up);
		}
		if (HAS_OBJECT(value, "right")) {
			PARSE_CHILD(result, value["right"], rt);
		}
	}
};

}


#endif //GLVIEW_RENDERLOCATION_H
