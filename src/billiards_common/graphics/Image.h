//
// Created by thallock on 9/21/21.
//

#ifndef IDEA_IMAGE_H
#define IDEA_IMAGE_H

#include "GraphicsPrimitive.h"

#include "billiards_common/geometry/Point.h"

namespace billiards::graphics {

	class Image : public GraphicsPrimitive {
	public:
		std::string path;
		geometry::Point lr;
		geometry::Point ur;
		geometry::Point ul;
		geometry::Point ll;

		Image()
			: path{}
			, lr{}
			, ur{}
			, ul{}
			, ll{}
		{}

		~Image() override = default;

		[[nodiscard]] std::string get_type() const override { return "image"; };

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_STRING(status, value, "path", "Images must have a path");
			path = value["path"].get<std::string>();
			REQUIRE_CHILD(status, value, "lower-right", lr, "Missing image bounds");
			REQUIRE_CHILD(status, value, "upper-right", ur, "Missing image bounds");
			REQUIRE_CHILD(status, value, "upper-left", ul, "Missing image bounds");
			REQUIRE_CHILD(status, value, "lower-left", ll, "Missing image bounds");
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			GraphicsPrimitive::to_json(writer);
			writer.field("path", path);
			writer.key("lower-right");
			lr.to_json(writer);
			writer.key("upper-right");
			ur.to_json(writer);
			writer.key("upper-left");
			ul.to_json(writer);
			writer.key("lower-left");
			ll.to_json(writer);
			writer.end_object();
		}
	};

}

#endif //IDEA_IMAGE_H
