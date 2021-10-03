//
// Created by thallock on 10/1/21.
//

#ifndef IDEA_SHOT_IS_POSSIBLE_H
#define IDEA_SHOT_IS_POSSIBLE_H


#include "billiards_common/config/Table.h"
#include "billiards_common/shots/Locations.h"
#include "billiards_common/shots/ShotInformation.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool shot_info_is_possible(
		const config::Table& table,
		const layout::Locations& locations,
		const ShotInformation& info
	) {
		// intersections...

		std::list<geometry::Point> previous_ghosts;
		auto *cs = (CueStep *) info.shot.steps[0].get();
		const auto& lb = locations.balls[cs->cue_ball];
		previous_ghosts.emplace_back(lb.location);

		for (int i = 0; i < info.destinations.size() - 1; i++) {
			const auto& step = info.shot.steps[info.destinations[i].step_index];

			const auto& current_posts = std::dynamic_pointer_cast<GoalPostTarget>(info.destinations[i].target);
			const auto& next_posts = std::dynamic_pointer_cast<GoalPostTarget>(info.destinations[i+1].target);

//				for (const auto& cur : current_posts) {
//					// check in bounds...
//				}

			for (const auto& prev : previous_ghosts) {
				for (const auto& cur : current_posts->posts()) {
					for (const auto& next : next_posts->posts()) {
						const auto dir1 = cur - prev;
						const auto dir2 = next - cur;
						const auto dot = dir1.dot(dir2);
						if (!dot.is_valid()) {
							return false;
						}
						switch (step->type) {
							case step_type::RAIL: {
								if (dot.get() > 0) {
									return false;
								}
								break;
							}
							case step_type::STRIKE: {
								if (dot.get() < 0) {
									return false;
								}
								break;
							}
							case step_type::CUE:
							case step_type::POCKET:
							case step_type::KISS:
								// TODO: Implement me
							case step_type::UNKNOWN:
								return false;
						}
					}
				}
			}

			switch (step->type) {
				case step_type::RAIL: {
					const auto& rail_step = info.get_typed_step<shots::RailStep>(info.destinations[i].step_index);
					const auto& rail = table.rail(rail_step->rail);
					for (const auto& post : current_posts->posts()) {
						const auto& dist = distance_to_segment(rail.segment1, rail.segment2, post);
						// TODO
						if (!dist.is_valid() || dist.get() > 1.13 + 1) {
							return false;
						}
					}

					if (info.shot.steps[i+1]->type == step_type::POCKET) {
						const auto& pocket_step = std::dynamic_pointer_cast<PocketStep>(info.shot.steps[i+1]);
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
}


#endif //IDEA_SHOT_IS_POSSIBLE_H
