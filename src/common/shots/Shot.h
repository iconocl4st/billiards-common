//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOT_H
#define GLVIEW_SHOT_H

#include <list>

#include "ShotStep.h"

namespace billiards::shots {

	class Shot : public json::Serializable {
	public:
		std::list<std::shared_ptr<ShotStep>> steps;

		Shot() = default;
		virtual ~Shot() = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("steps");
			writer.begin_array();
			for (const auto& step : steps) {
				step->to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			steps.clear();
			if (value.contains("steps") && value["steps"].is_array()) {
				for (const auto& it : value["steps"]) {
					steps.emplace_back(step::parse(it));
				}
			}
		}
	};
}

#endif //GLVIEW_SHOT_H
