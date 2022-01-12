//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_ROLLING_KISS_H
#define IDEA_ROLLING_KISS_H

#include "AssignmentStatus.h"
#include "math/rolling_glance.h"
#include "math/rolling_glance_to_pocket.h"
#include "billiards_common/shots/shot_helpers.h"

#include "pocket.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign_rolling_kiss_locations(AssignmentStatus& status) {
		if (status.already_has_source() && status.already_has_target()) {
			return false;
		}
		const auto& prev = get_previous_status(status);
		if (!prev.already_has_source()) {
			return false;
		}
		const auto& cue_loc = prev.exiting_location;
		const auto& cue_radius = prev.exiting_radius;
		const auto& kiss_step = status.info.get_typed_step<KissStep>(status.get_info());
		const auto& obj_loc = status.params.locations.balls[kiss_step->kissed_ball].location;
		const auto& obj_radius = get_ball_type(status.params.config, status.params.locations, kiss_step->kissed_ball)->radius;

		const auto& next_status = get_next_status(status);
		const auto next_step_type = status.info.get_shot_type(next_status.index);

		if (next_status.already_has_target() && next_step_type != step_type::POCKET) {
			const auto& next_target = next_status.get_target();
			auto target = std::make_shared<GoalPostTarget>();
			for (int i = 0; i < 3; i++) {
				bool assigned = false;
				billiards::shots::math::get_glance_location(
					cue_loc - obj_loc,
					next_target->posts[i] - obj_loc,
					(cue_radius + obj_radius).get(),
					[&](const RollingGlanceCalculation& c) {
						assigned = true;
						target->posts[i] = geometry::MaybePoint{c.loc.x, c.loc.y} + obj_loc;

						if (i == 1) {
							status.get_info().rolling_calculation = std::make_shared<RollingGlanceCalculation>(
								c.add_object(obj_loc));
						}
					}
				);

				if (!assigned) {
					status.info.update_validity(false);
					return false;
				}
			}
			status.assign_target(target);
			// TODO:
			status.assign_exiting_location(target->posts[1]);
			return true;
		}

		if (next_step_type != step_type::POCKET) {
			return false;
		}

		const auto& pocket_step = status.info.get_typed_step<PocketStep>(next_status.get_info());
		const auto pockets = status.params.config.pockets();
		const auto& pocket = pockets[pocket_step->pocket];
//		const auto& pocket = status.params.config.get_pocket(pocket_step->pocket);

		const auto next_target = std::make_shared<GoalPostTarget>();
		auto target = std::make_shared<GoalPostTarget>();

		bool assigned = false;
		billiards::shots::math::solve_glance_and_pocket(
			cue_loc - obj_loc,
			pocket.outer_segment_1 - obj_loc,
			pocket.outer_segment_2 - obj_loc,
			cue_radius.get(), obj_radius,
			[&](const math::KissToPocketSolution& sol) {
				assigned = true;
				target->posts[0] = geometry::MaybePoint{sol.x, sol.y} + obj_loc;
				next_target->posts[0] = geometry::MaybePoint{sol.dx, sol.dy} + obj_loc;
//				status.get_info().rolling_calculation = std::make_shared<RollingGlanceCalculation>(sol.calc());
			}
		);
		status.info.update_validity(assigned);
		assigned = false;
		billiards::shots::math::solve_glance_and_pocket(
			cue_loc - obj_loc,
			pocket.outer_segment_2 - obj_loc,
			pocket.outer_segment_1 - obj_loc,
			cue_radius.get(), obj_radius,
			[&](const math::KissToPocketSolution& sol) {
				assigned = true;
				target->posts[2] = geometry::MaybePoint{sol.x, sol.y} + obj_loc;
				next_target->posts[2] = geometry::MaybePoint{sol.dx, sol.dy} + obj_loc;
			}
		);
		status.info.update_validity(assigned);
		const auto& d = (next_target->posts[0] + next_target->posts[2]) / 2.0;
		status.info.update_validity(d.is_valid());
		if (d.is_valid()) {
			assigned = false;
			billiards::shots::math::get_glance_location(
				cue_loc - obj_loc,
				d - obj_loc,
				(cue_radius + obj_radius).get(),
				[&](const RollingGlanceCalculation& c) {
					assigned = true;
					target->posts[1] = geometry::MaybePoint{c.loc.x, c.loc.y} + obj_loc;
					status.get_info().rolling_calculation = std::make_shared<RollingGlanceCalculation>(
						c.add_object(obj_loc));
				}
			);
			status.info.update_validity(assigned);
		}

		status.assign_target(target);
		tidy_pocket_posts(status, target, pocket);
		// TODO:
		status.assign_exiting_location(target->posts[1]);
		return true;
	}
}

#endif //IDEA_ROLLING_GLANCE_H
