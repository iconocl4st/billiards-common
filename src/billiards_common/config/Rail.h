//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_RAIL_H
#define IDEA_RAIL_H

#include "billiards_common/utils/Serializable.h"

namespace billiards::config {
	class Rail /*: public json::Serializable*/ {
	public:
		geometry::Point segment1;
		geometry::Point segment2;
		geometry::Point in;

		Rail(const geometry::Point& s1, const geometry::Point& s2, const geometry::Point& in)
			: segment1{s1}
			, segment2{s2}
			, in{in}
		{}
		~Rail() = default;

		/*
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.end_object();
		};

		void parse(const nlohmann::json& value) override {

		};*/
	};
}
#endif //IDEA_RAIL_H
