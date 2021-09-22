//
// Created by thallock on 9/21/21.
//

#ifndef IDEA_IMAGE_H
#define IDEA_IMAGE_H

#include "./GraphicsPrimitive.h"

#include "../geometry/Point.h"

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

		void parse(const nlohmann::json& value) override {
			if (value.contains("path") && value["path"].is_string()) {
				path = value["path"].get<std::string>();
			}
			if (value.contains("lower-right") && value["lower-right"].is_object()) {
				lr.parse(value["lower-right"]);
			}
			if (value.contains("upper-right") && value["upper-right"].is_object()) {
				ur.parse(value["upper-right"]);
			}
			if (value.contains("upper-left") && value["upper-left"].is_object()) {
				ul.parse(value["upper-left"]);
			}
			if (value.contains("lower-left") && value["lower-left"].is_object()) {
				ll.parse(value["lower-left"]);
			}
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
