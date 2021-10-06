//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_CUEINGINFO_H
#define IDEA_CUEINGINFO_H

#include <memory>

#include "billiards_common/geometry/Maybe.h"
//#include "billiards_common/shots/Locations.h"
#include "billiards_common/shots/Target.h"

namespace billiards::shots {

	// Rename to StepInfo
		class StepInfo : public billiards::json::Serializable {
		public:
			int step_index;

			std::shared_ptr<GoalPostTarget> target;


			StepInfo() = default;
			StepInfo(int step_index) : step_index{step_index}, target{nullptr} {}
			virtual ~StepInfo() = default;

//			void assign_exiting(const layout::Locations& locations, int ball_index) {
//				exiting_source = locations.balls[ball_index].location;
//				exiting_type = locations.balls[ball_index].ball;
//			}

			void parse(const nlohmann::json& value, json::ParseResult& status) override {
				ENSURE_OBJECT(status, value, "target", "StepInfo must have a target");
				target = targets::parse(value["target"], status);
				if (!status.success) {
					return;
				}
				ENSURE_NUMBER(status, value, "step-index", "StepInfo must have a step index");
				step_index = value["step-index"].get<int>();
			}

			void to_json(billiards::json::SaxWriter& writer) const override {
				writer.begin_object();
				writer.field("step-index", step_index);
				writer.key("target");
				target->to_json(writer);
				writer.end_object();
			}
		};
}


#endif //IDEA_CUEINGINFO_H
