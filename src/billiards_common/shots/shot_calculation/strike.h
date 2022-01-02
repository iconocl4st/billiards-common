//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_STRIKE_H
#define IDEA_STRIKE_H

#include "AssignmentStatus.h"
#include "math/basic_calculations.h"
//#include "billiards_common/geometry/geometry.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign_strike_source(AssignmentStatus& status) {
		if (status.already_has_source()) {
			return false;
		}
		const auto& strike_step = status.info.get_typed_step<StrikeStep>(status.get_info());
		status.assign_exiting(strike_step->object_ball);
		return true;
	}

	[[nodiscard]] inline
	bool maybe_assign_strike_target(AssignmentStatus& status) {
		if (status.already_has_target()) {
			return false;
		}
        const auto& next_status = get_next_status(status);
		if (!get_previous_status(status).assigned_exiting_radius ||
			!next_status.already_has_target()) {
			return false;
		}
		if (!status.already_has_source()) {
			return false;
		}

		const auto& strike_step = status.info.get_typed_step<StrikeStep>(status.get_info());
		const auto& cue_radius = status.ass.statuses[status.index - 1].exiting_radius;
		const auto& object_loc = status.exiting_location;
		const auto& object_radius = status.exiting_radius;

		auto target = std::make_shared<GoalPostTarget>();
		auto& next_target = next_status.get_target();
		for (int i = 0; i < 3; i++) {
			target->posts[i] = billiards::geometry::extend_line(
				next_target->posts[i],
				object_loc,
				object_radius + cue_radius);
		}
		// TODO: Cannot be out of bounds...
		// Need to project this onto to the table at a certain distance...
		status.assign_target(target);
		return true;
	}


	[[nodiscard]] inline
	bool maybe_assign_strike(AssignmentStatus& status) {
		const bool b1 = maybe_assign_strike_source(status);
		const bool b2 = maybe_assign_strike_target(status);
		return b1 || b2;
	}
}

#endif //IDEA_STRIKE_H
