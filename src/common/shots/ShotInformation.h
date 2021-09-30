//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOTINFORMATION_H
#define GLVIEW_SHOTINFORMATION_H

#include "./Shot.h"
#include "./ShotStepInfo.h"

namespace billiards::shots {

	class ShotInformation : public json::Serializable {
	public:
		Shot shot;
		CueingInfo cueing;
		std::vector<Destination> destinations;

		[[nodiscard]] inline
		std::shared_ptr<ShotStep>& get_step(const Destination& dest) {
			const int step_index = dest.step_index;
			if (step_index < 0 || step_index >= shot.steps.size()) {
				throw std::runtime_error{"Invalid index"};
			}
			return shot.steps[step_index];
		}

		[[nodiscard]] inline
		const std::shared_ptr<ShotStep>& get_step(const Destination& dest) const {
			const int step_index = dest.step_index;
			if (step_index < 0 || step_index >= shot.steps.size()) {
				throw std::runtime_error{"Invalid index"};
			}
			return shot.steps[step_index];
		}

		template<class StepType>
		std::shared_ptr<StepType> get_typed_step(const Destination& dest) {
			return std::dynamic_pointer_cast<StepType>(get_step(dest));
		}

		[[nodiscard]] inline
		step_type::ShotStepType get_shot_type(const Destination& dest) const {
			return get_step(dest)->type;
		}

		[[nodiscard]] inline
		step_type::ShotStepType get_shot_type(int dest_index) const {
			if (dest_index < 0 || dest_index >= destinations.size()) {
				throw std::runtime_error{"Invalid destination index"};
			}
			return get_shot_type(destinations[dest_index]);
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("shot");
			shot.to_json(writer);
			writer.key("cueing");
			cueing.to_json(writer);
			writer.key("destinations");
			writer.begin_array();
			for (const auto& dest : destinations) {
				dest.to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("shot") && value["shot"].is_object()) {
				shot.parse(value["shot"]);
			}
			if (value.contains("cueing") && value["cueing"].is_object()) {
				cueing.parse(value["cueing"]);
			}
			if (value.contains("destinations") && value["destinations"].is_array()) {
				destinations.clear();
				for (const auto& it : value["destinations"]) {
					destinations.emplace_back();
					destinations.back().parse(it);
				}
			}

//			// This should have been cleaner, maybe the should only have an index?
//			for (int i = 0; i < destinations.size(); i++) {
//				auto dest = destinations[i];
//				auto step = shot.steps[i+1];
//				switch (dest->type) {
//					case dest_type::GHOST_BALL: {
//						auto d = std::dynamic_pointer_cast<GhostBallDestination>(dest);
//						auto s = std::dynamic_pointer_cast<StrikeStep>(step);
//						d->step = s;
//					}
//					case dest_type::POCKET: {
//						auto d = std::dynamic_pointer_cast<PocketDestination>(dest);
//						auto s = std::dynamic_pointer_cast<PocketStep>(step);
//						d->step = s;
//					}
//					case dest_type::UNKNOWN:
//					default:
//						throw std::runtime_error{"Unknown type"};
//				}
//			}
		}

		[[nodiscard]] bool is_possible() const {
			// TODO:
			return true;
		}
	};
}


#endif //GLVIEW_SHOTINFORMATION_H
