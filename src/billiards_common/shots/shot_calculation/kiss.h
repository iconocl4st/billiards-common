//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_GLANCE_H
#define IDEA_GLANCE_H

#include "billiards_common/shots/shot_calculation/rolling_kiss.h"
#include "stun_kiss.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign_kiss_source_radius(AssignmentStatus& status) {
		if (status.assigned_exiting_radius) {
			return false;
		}

		const auto& prev = get_previous_status(status);
		if (!prev.assigned_exiting_radius) {
			return false;
		}

		status.assign_exiting_radius(prev.exiting_radius);
		return true;
	}


	[[nodiscard]] inline
	bool maybe_assign_kiss_locations(AssignmentStatus& status) {
		const auto& kiss_step = status.info.get_typed_step<KissStep>(status.get_info());
		switch (kiss_step->type) {
			case kt::ROLLING:
				return maybe_assign_rolling_kiss_locations(status);
			case kt::STUN:
				return maybe_assign_stun_kiss_locations(status);
			case kt::UNKNOWN:
				return false;
		}
	}

	[[nodiscard]] inline
	bool maybe_assign_kiss(AssignmentStatus& status) {
		const bool b1 = maybe_assign_kiss_source_radius(status);
		const bool b2 = maybe_assign_rolling_kiss_locations(status);
		return b1 || b2;
	}
}

#endif //IDEA_GLANCE_H
