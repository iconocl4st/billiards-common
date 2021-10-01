//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOTINFORMATION_H
#define GLVIEW_SHOTINFORMATION_H

#include "./Shot.h"
#include "./CueingInfo.h"

#include "common/config/Table.h"

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
		std::shared_ptr<StepType> get_typed_step(const Destination& dest) const {
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
//				auto step = shot.steps[i+1];src/test_bank.cpp
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

		[[nodiscard]] bool is_possible(const config::Table& table, const layout::Locations& locations) const {
			// intersections...

			std::list<geometry::Point> previous_ghosts;
			auto *cs = (CueStep *) shot.steps[0].get();
			const auto& lb = locations.balls[cs->cue_ball];
			previous_ghosts.emplace_back(lb.location);

			for (int i = 0; i < destinations.size() - 1; i++) {
				const auto& step = shot.steps[destinations[i].step_index];

				const auto& current_posts = std::dynamic_pointer_cast<GoalPostTarget>(destinations[i].target);
				const auto& next_posts = std::dynamic_pointer_cast<GoalPostTarget>(destinations[i+1].target);

//				for (const auto& cur : current_posts) {
//					// check in bounds...
//				}

				for (const auto& prev : previous_ghosts) {
					for (const auto& cur : current_posts->posts()) {
						for (const auto& next : next_posts->posts()) {
							auto dir1 = cur - prev;
							auto dir2 = next - cur;
							auto dot = dir1.dot(dir2);
							if (!dot.is_valid() || dot.get() < 0) {
								return false;
							}
						}
					}
				}

				switch (step->type) {
					case step_type::RAIL: {
						const auto& rail_step = get_typed_step<shots::RailStep>(destinations[i].step_index);
						const auto& rail = table.rail(rail_step->rail);
						for (const auto& post : current_posts->posts()) {
							const auto& dist = distance_to_segment(rail.segment1, rail.segment2, post);
							// TODO
							if (!dist.is_valid() || dist.get() > 1.13 + 1) {
								return false;
							}
						}

						if (shot.steps[i+1]->type == step_type::POCKET) {
							const auto& pocket_step = std::dynamic_pointer_cast<PocketStep>(shot.steps[i+1]);
							if (table.is_incident(pocket_step->pocket, rail_step->rail)) {
								return false;
							}
						}
					}
				}

				previous_ghosts.clear();
				for (const auto& cur : current_posts->posts()) {
					previous_ghosts.emplace_back(cur.point());
				}
			}
//			for (const auto& step : shot.steps) {
//				switch (step->type) {
//					case step_type::RAIL: {
//
//					}
//					case step_type::CUE: {
//
//					}
//					case step_type::KISS: {
//
//					}
//					case step_type::POCKET: {
//
//					}
//					case step_type::STRIKE: {
////
////						// simple shot only...
////
////						auto *ss = (StrikeStep *) shot.steps[1].get();
////						auto *ps = (PocketStep *) shot.steps[2].get();
////						auto& cue_loc = locations.balls[cs->cue_ball].location;
////						auto& obj_loc = locations.balls[ss->object_ball].location;
////						auto poc_loc = table.pockets[ps->pocket].center();
////
////						auto dir1 = obj_loc - cue_loc;
////						auto dir2 = poc_loc - obj_loc;
////
////						auto dot = dir1.dot(dir2);
////						return dot > 0;
//					}
//					case step_type::UNKNOWN: {
//
//					}
//				}
//			}
			// TODO:
			return true;
		}
	};
}


#endif //GLVIEW_SHOTINFORMATION_H
