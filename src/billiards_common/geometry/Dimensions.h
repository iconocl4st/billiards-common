//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_TABLEDIMENSIONS_H
#define GLVIEW_TABLEDIMENSIONS_H

#include "billiards_common/utils/Serializable.h"

namespace billiards::geometry {

	class Dimensions : public json::Serializable {
	public:
		double width;
		double height;

		Dimensions(double w, double h) : width{w}, height{h} {}
		Dimensions() : width{-1}, height{-1} {}
//		Dimensions() : width{92}, height{46} {}

		virtual ~Dimensions() = default;
		
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("width", width);
			writer.field("height", height);
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_NUMBER(status, value, "width", "Dimensions must have a width");
			width = value["width"].get<double>();
			ENSURE_NUMBER(status, value, "width", "Dimensions must have a height");
			height = value["height"].get<double>();
		}
	};

}


#endif //GLVIEW_TABLEDIMENSIONS_H
