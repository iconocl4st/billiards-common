//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_CUEING_H
#define IDEA_CUEING_H

#include "AssignmentStatus.h"

namespace billiards::shots {

	[[nodiscard]] inline
	geometry::MaybeDouble calculate_precision(const Assignment& ass) {
		// a . b = |a| * |b| cos theta
		const auto p1 = ass.statuses[1].get_target()->posts[0] - ass.statuses[0].exiting_location;
		const auto p2 = ass.statuses[1].get_target()->posts[2] - ass.statuses[0].exiting_location;
		auto cos_theta = p1.dot(p2) / p1.norm() / p2.norm();
		return cos_theta.acos();
	}

	[[nodiscard]] inline
	std::shared_ptr<std::array<double, 3>> get_target_cut(const Assignment& ass) {
		const auto cue_location = geometry::MaybePoint{ass.info.cueing.cue_location};
		const auto& cue_radius = ass.info.cueing.cue_radius;

		geometry::MaybeDouble object_radius;
		geometry::MaybePoint object_location;
		if (ass.info.infos.size() <= 1) {
			return nullptr;
		}
		switch (ass.info.get_shot_type(1)) {
			case step_type::STRIKE: {
				const auto& strike_step = ass.info.get_typed_step<StrikeStep>(ass.statuses[1].get_info());
				object_radius = get_ball_type(ass.params.config, ass.params.locations, strike_step->object_ball)->radius;
				object_location = ass.params.locations.balls[strike_step->object_ball].location;
				break;
			}
			case step_type::KISS: {
				const auto& kiss_step = ass.info.get_typed_step<KissStep>(ass.statuses[1].get_info());
				object_radius = get_ball_type(ass.params.config, ass.params.locations, kiss_step->kissed_ball)->radius;
				object_location = ass.params.locations.balls[kiss_step->kissed_ball].location;
				break;
			}
			case step_type::POCKET:
			case step_type::RAIL:
			case step_type::CUE:
			case step_type::UNKNOWN:
			default:
				return nullptr;
		}
		if (!object_location.is_valid() || !object_radius.is_valid()) {
			return nullptr;
		}
		const auto direct_line = geometry::through(cue_location, object_location);
		const auto orthog_line = orthogonal_at(direct_line, object_location);

		if (((object_location - cue_location).norm() < TOLERANCE).get_or(true)) {
			return nullptr;
		}

		const auto& target = ass.info.infos[1].target;
		auto ret = std::make_shared<std::array<double, 3>>();
		for (int i = 0; i < 3; i++) {
			// for each target...
			const auto aim_line = geometry::through(cue_location, target->posts[i]);
			const auto inter = geometry::intersection(aim_line, orthog_line);
			const auto dist = (inter - object_location).norm();
			const auto to_the_right = geometry::is_to_the_right_of(cue_location - object_location, target->posts[i] - cue_location);
			const auto sign = geometry::ternary(to_the_right, -1, 1);
			const auto cut = sign * dist / object_radius;
			if (!cut.is_valid()) {
				return nullptr;
			}
			ret->at(i) = cut.get();
		}
		return ret;
	}

	inline
	void assign_cueing(Assignment& ass) {
		ass.info.cueing.cue_location = ass.statuses[0].exiting_location.get();
		ass.info.cueing.cue_radius = ass.statuses[0].exiting_radius;
		ass.info.cueing.precision = calculate_precision(ass);
		ass.info.cueing.cuts = get_target_cut(ass);
	}
}

#endif //IDEA_CUEING_H
