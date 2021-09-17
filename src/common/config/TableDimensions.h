//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_TABLEDIMENSIONS_H
#define GLVIEW_TABLEDIMENSIONS_H

#include "common/utils/Serializable.h"

namespace billiards::config {

	class TableDimensions : public json::Serializable {
	public:
		double width;
		double height;

		TableDimensions(double w, double h) : width{w}, height{h} {}
		TableDimensions() : width{92}, height{46} {}

		virtual ~TableDimensions() = default;
		
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("width", width);
			writer.field("height", height);
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value) override {
			if (value.contains("width") && value["width"].is_number()) {
				width = value["width"].get<double>();
			}
			if (value.contains("height") && value["height"].is_number()) {
				height = value["height"].get<double>();
			}
		}
	};

}


#endif //GLVIEW_TABLEDIMENSIONS_H
