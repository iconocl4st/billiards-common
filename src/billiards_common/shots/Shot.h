//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOT_H
#define GLVIEW_SHOT_H

#include <vector>

#include "ShotStep.h"

namespace billiards::shots {

	class Shot : public json::Serializable {
	public:
		std::vector<std::shared_ptr<ShotStep>> steps;

		Shot() = default;
		virtual ~Shot() = default;

		void operator=(const Shot& other) {
			steps.clear();

			for (const auto& it : other.steps) {
				steps.push_back(it);
			}
		}

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

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_ARRAY(status, value, "steps", "Shot must have steps");
			steps.clear();
			for (const auto& it: value["steps"]) {
				auto step = step::parse(it, status);
				if (!status.success) {
					return;
				}
				steps.push_back(step);
			}
		}
	};
}

#endif //GLVIEW_SHOT_H
