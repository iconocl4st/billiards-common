//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_STEP_INFO_H
#define IDEA_STEP_INFO_H

#include "shot_calculation/cue.h"
#include "shot_calculation/kiss.h"
#include "shot_calculation/rail.h"
#include "shot_calculation/strike.h"
#include "shot_calculation/pocket.h"

namespace billiards::shots {


		/*
//		const int step_index = info.destinations[index].step_index;
		const auto& dest = info.destinations[index];

		bool has_source = false;
		const geometry::MaybePoint source;
		if (index > 0 && assigned[index - 1].assigned_source) {
			source = info.destinations[index - 1].exiting_source;
			has_source = true;
		}
		bool has_target = false;
		const std::shared_ptr<GoalPostTarget> target;
		if (index < info.destinations.size() - 1 && assigned[index + 1].assigned_target) {
			target = info.destinations[index + 1].target;
			has_target = true;
		}*/


/*



			case billiards::shots::step_type::RAIL: {
				return progress;
			}
			case billiards::shots::step_type::POCKET: {
				bool progress = false;
				// No source
				if (!assigned[index].assigned_source) {
					assigned[index].assigned_source = true;
					progress = true;
				}
				if (!assigned[index].assigned_target) {
					if (has_source) {
						// orient pocket
						assigned[index].assigned_target = true;
						progress = true;
					}
				}
				return progress;
			}
			case billiards::shots::step_type::CUE: {
				bool progress = false;
				// No source
				if (!assigned[index].assigned_source) {
					const auto& cue_step = info.get_typed_step<CueStep>(dest);
					dest.exiting_source = params.locations.get_ball_location(cue_step->cue_ball);
					assigned[index].assigned_source = true;
					progress = true;
				}
				// No target
				if (!assigned[index].assigned_target) {
					assigned[index].assigned_target = true;
					progress = true;
				}
				return progress;
			}
			case billiards::shots::step_type::STRIKE: {
				bool progress = false;
				const auto& strike_step = info.get_typed_step<StrikeStep>(dest);
				const auto& obj_loc = params.locations.get_ball_location(strike_step->object_ball);
				if (!assigned[index].assigned_source) {
					dest.exiting_source = obj_loc;
					progress = true;
				}
				if (!assigned[index].assigned_target) {
					if (has_source && has_target) {
						auto target = std::make_shared<GoalPostTarget>();
						dest.target = std::dynamic_pointer_cast<Target>(target);
						// calculate strike
						progress = true;
					}
				}
				return progress;
			}
			case billiards::shots::step_type::ROLLING_GLANCE: {
				const auto& glance_step = info.get_typed_step<GlanceStep>(dest);
				const auto& obj_loc = params.locations.get_ball_location(glance_step->object_ball);
				if (assigned[index].assigned_source && assigned[index].assigned_target) {
					return false;
				}

				auto target = std::make_shared<GoalPostTarget>();
				dest.target = std::dynamic_pointer_cast<Target>(target);
				if (!has_source) {
					return false;
				}
				if (has_target) {
					assigned[index].assigned_source = true;
					assigned[index].assigned_target = true;
					return true;
				}
				if (!next type is a pocket) {
					return false;
				}
				assigned[index].assigned_source = true;
				assigned[index].assigned_target = true;

				assigned[index + 1].assigned_source = true;
				assigned[index + 1].assigned_target = true;
				return true;
			}
			case billiards::shots::step_type::STUN_GLANCE: {

			}
			case billiards::shots::step_type::UNKNOWN: {
				break;
			}
		}
		*/
}

#endif //IDEA_STEP_INFO_H
