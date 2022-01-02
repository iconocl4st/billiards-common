//
// Created by thallock on 12/24/21.
//

#ifndef IDEA_TABLEDIMENSIONS_H
#define IDEA_TABLEDIMENSIONS_H

#include "billiards_common/geometry/Dimensions.h"


namespace billiards::config {

	class TableDimensions : public json::Serializable {
	public:
		double width;
		double height;
		double pocket_width;

		TableDimensions(double w, double h, double pw)
			: width{w}
			, height{h}
			, pocket_width{pw}
		{}

		TableDimensions() : TableDimensions{92, 46, 4} {}

		~TableDimensions() override = default;

		[[nodiscard]] inline
		geometry::Dimensions dims() const {
			return geometry::Dimensions{width, height};
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("width", width);
			writer.field("height", height);
			writer.field("pocket-width", pocket_width);
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_NUMBER(status, value, "width", "Dimensions must have a width");
			width = value["width"].get<double>();
			ENSURE_NUMBER(status, value, "width", "Dimensions must have a height");
			height = value["height"].get<double>();
			ENSURE_NUMBER(status, value, "width", "Dimensions must have a pocket-width");
			pocket_width = value["pocket-width"].get<double>();
		}

	};

}

#endif //IDEA_TABLEDIMENSIONS_H
