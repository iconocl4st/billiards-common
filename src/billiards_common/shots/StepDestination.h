//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_CUEINGINFO_H
#define IDEA_CUEINGINFO_H

#include <memory>

#include "billiards_common/shots/Target.h"
#include "billiards_common/shots/RollingGlanceCalculation.h"

namespace billiards::shots {

	// Rename file
		class StepInfo : public billiards::json::Serializable {
		public:
			int step_index;

			std::shared_ptr<GoalPostTarget> target;
			std::shared_ptr<RollingGlanceCalculation> rolling_calculation;

			explicit StepInfo(int step_index) : step_index{step_index}, target{nullptr}, rolling_calculation{nullptr} {}
			StepInfo() : StepInfo(-1) {}
			~StepInfo() override = default;

			void parse(const nlohmann::json& value, json::ParseResult& status) override {
				ENSURE_NUMBER(status, value, "step-index", "StepInfo must have a step index");
				step_index = value["step-index"].get<int>();
				if (step_index < 0) {
					///
				}
				if (HAS_OBJECT(value, "target")) {
					target = std::make_shared<GoalPostTarget>();
					PARSE_CHILD(status, value["target"], (*target));
				}
				if (HAS_OBJECT(value, "rolling-glance-calculation")) {
					rolling_calculation = std::make_shared<RollingGlanceCalculation>();
					PARSE_CHILD(status, value["rolling-glance-calculation"], (*rolling_calculation));
				}
			}

			void to_json(billiards::json::SaxWriter& writer) const override {
				writer.begin_object();
				writer.field("step-index", step_index);
				if (target) {
					writer.key("target");
					target->to_json(writer);
				}
				if (rolling_calculation) {
					writer.key("rolling-glance-calculation");
					rolling_calculation->to_json(writer);
				}
				writer.end_object();
			}
		};
}


#endif //IDEA_CUEINGINFO_H
