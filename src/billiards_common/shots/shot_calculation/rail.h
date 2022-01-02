//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_RAIL_CALC_H
#define IDEA_RAIL_CALC_H

#include "AssignmentStatus.h"
#include "math/bank.h"
#include "billiards_common/geometry/geometry.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign_rail_source_radius(AssignmentStatus& status) {
		if (status.assigned_exiting_radius) {
			return false;
		}

		const auto& prev = get_previous_status(status);
		if (prev.assigned_exiting_radius) {
			return false;
		}

		status.assign_exiting_radius(prev.exiting_radius);
		return true;
	}

	[[nodiscard]] inline
	bool maybe_assign_rail_source_location(AssignmentStatus& status) {
		if (status.already_has_source()) {
			return false;
		}
		bool progress = false;
		const auto previous = get_previous_status(status);
		if (!status.assigned_exiting_radius && previous.assigned_exiting_radius) {
			status.assign_exiting_radius(previous.exiting_radius);
			progress = true;
		}
		if (!previous.assigned_exiting_loc) {
			return progress;
		}
		const auto& rail_step = status.info.get_typed_step<RailStep>(status.get_info());
		const auto& rail = status.params.config.rail(rail_step->rail);
		const auto& source = previous.exiting_location;
		const auto& radius = previous.exiting_radius;

		const auto rail_line = geometry::through(rail.segment1, rail.segment2);
		const auto reflection = geometry::reflect(source, rail_line);
		// Could use reflection...
		const auto travel_line = geometry::through(source, reflection);
		const auto bank_line = geometry::parallel_at(rail_line, rail.segment1 + rail.in * radius.get());
		const auto intersection = geometry::intersection(bank_line, travel_line);
		status.assign_exiting_location(intersection);

//		const auto& rail_step = info.get_typed_step<RailStep>(index);
//		info.destinations[index].exiting_source = params.locations.get_ball_location(rail_step->cue_ball);
//		statuses.exiting_source_assigned(index);
		return true;
	}

	[[nodiscard]] inline
	bool maybe_assign_rail_target(AssignmentStatus& status) {
		if (status.already_has_target()) {
			return false;
		}
		if (!get_previous_status(status).already_has_source() || !get_next_status(status).already_has_target()) {
			return false;
		}

		const auto& rail_step = status.info.get_typed_step<RailStep>(status.get_info());
		const auto& rail = status.params.config.rail(rail_step->rail);
		const auto& source = status.ass.statuses[status.index - 1].exiting_location;
		const auto& radius = status.ass.statuses[status.index - 1].exiting_radius;
		const auto& next_target = get_next_status(status).get_target();
		std::shared_ptr<GoalPostTarget> target = std::make_shared<GoalPostTarget>();
		for (int i = 0; i < 3; i++) {
			target->posts[i] = shots::math::calculate_bank(
				source, next_target->posts[0],
				rail.segment1, rail.segment2, rail.in,
				radius);
		}
		status.assign_target(target);
		return true;
	}

	[[nodiscard]] inline
	bool maybe_assign_rail(AssignmentStatus& status) {
		const bool b1 = maybe_assign_rail_source_radius(status);
		const bool b2 = maybe_assign_rail_source_location(status);
		const bool b3 = maybe_assign_rail_target(status);
		return b1 || b2 || b3;
	}
}

#endif //IDEA_RAIL_H
