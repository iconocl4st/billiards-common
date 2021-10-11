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

		std::shared_ptr<GoalPostTarget> previous_ghosts = std::make_shared<GoalPostTarget>();
		auto *cs = (CueStep *) info.shot.steps[0].get();
		for (int i = 0; i < 3; i++) {
			previous_ghosts->posts[i] = locations.balls[cs->cue_ball].location;
		}
		for (int i = 1; i < info.infos.size() - 1; i++) {
			const auto& step = info.get_step(info.infos[i]);

			const auto& current_posts = info.get_info(i).target;
			const auto& next_posts = info.get_info(i + 1).target;

//				for (const auto& cur : current_posts) {
//					// check in bounds...
//				}

			for (int j = 0; j < 3; j++) {
				if (!previous_ghosts || !current_posts || !next_posts) {
					return false;
				}
				const auto& prev = previous_ghosts->posts[j];
				const auto& cur = current_posts->posts[j];
				const auto& next = next_posts->posts[j];
				if (!prev.is_valid() || !cur.is_valid() || !next.is_valid()) {
					return false;
				}

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
						break;
					case step_type::UNKNOWN:
						return false;
				}
			}

			switch (step->type) {
				case step_type::RAIL: {
					const auto& rail_step = info.get_typed_step<shots::RailStep>(info.get_info(i));
					const auto& rail = table.rail(rail_step->rail);
					for (const auto& post : current_posts->posts) {
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

			previous_ghosts = current_posts;
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
