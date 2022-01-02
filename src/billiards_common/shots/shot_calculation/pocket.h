//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_POCKET_H
#define IDEA_POCKET_H

#include "AssignmentStatus.h"
#include "math/orient_pocket.h"

namespace billiards::shots {

	inline
	void tidy_pocket_posts(AssignmentStatus& status, std::shared_ptr<GoalPostTarget>& target, const config::Pocket& pocket) {
		target->posts[1] = (target->posts[0] + target->posts[2]) / 2.0;
		status.assign_target(target);

		const bool valid = target->posts[0].is_valid() && target->posts[2].is_valid();
		status.info.update_validity(valid);
		if (!valid) {
			return;
		}
		const auto gp11_dist = (pocket.outer_segment_1 - target->posts[0].get()).norm2();
		const auto gp12_dist = (pocket.outer_segment_1 - target->posts[2].get()).norm2();
		const auto gp21_dist = (pocket.outer_segment_2 - target->posts[0].get()).norm2();
		const auto gp22_dist = (pocket.outer_segment_2 - target->posts[2].get()).norm2();
		status.info.update_validity(gp11_dist <= gp12_dist && gp22_dist <= gp21_dist);
	}

	[[nodiscard]] inline
	bool maybe_assign_pocket_source(AssignmentStatus& status) {
		if (status.already_has_source()) {
			return false;
		}
		status.no_source();
		return true;
	}

	[[nodiscard]] inline
	bool maybe_assign_pocket_target(AssignmentStatus& status) {
		if (status.already_has_target()) {
			return false;
		}
		if (!get_previous_status(status).already_has_source()) {
			return false;
		}

		const std::shared_ptr<PocketStep>& pocket_step = status.info.get_typed_step<PocketStep>(status.get_info());
		const auto& pocket = status.params.config.get_pocket(pocket_step->pocket);
		const auto& source = get_previous_status(status).exiting_location;
		const auto& radius = get_previous_status(status).exiting_radius;

		std::shared_ptr<GoalPostTarget> target = std::make_shared<GoalPostTarget>();
		target->posts[0] = billiards::shots::math::orient_pocket(
			source, pocket.outer_segment_1, pocket.outer_segment_2, radius);
		target->posts[2] = billiards::shots::math::orient_pocket(
			source, pocket.outer_segment_2, pocket.outer_segment_1, radius);
		target->posts[1] = (target->posts[0] + target->posts[2]) / 2;

		tidy_pocket_posts(status, target, pocket);
		return true;
	}


	[[nodiscard]] inline
	bool maybe_assign_pocket(AssignmentStatus& status) {
		const bool b1 = maybe_assign_pocket_source(status);
		const bool b2 = maybe_assign_pocket_target(status);
		return b1 || b2;
	}
}

#endif //IDEA_POCKET_H
