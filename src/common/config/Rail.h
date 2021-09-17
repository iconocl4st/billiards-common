//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_RAIL_H
#define IDEA_RAIL_H

#include "common/utils/Serializable.h"

namespace billiards::config {
	class Rail : public json::Serializable {
	public:

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.end_object();
		};

		void parse(const nlohmann::json& value) override {

		};
	};
}
#endif //IDEA_RAIL_H
