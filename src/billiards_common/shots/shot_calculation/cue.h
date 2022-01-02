//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_CUE_H
#define IDEA_CUE_H

#include "AssignmentStatus.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign_cue_source(AssignmentStatus& status) {
		if (status.already_has_source()) {
			return false;
		}

		const auto& cue_step = status.info.get_typed_step<CueStep>(status.get_info());
		status.assign_exiting(cue_step->cue_ball);
		return true;
	}

	[[nodiscard]] inline
	bool maybe_assign_cue_target(AssignmentStatus& status) {
		if (status.already_has_target()) {
			return false;
		}
		status.no_target();
		return true;
	}


	[[nodiscard]] inline
	bool maybe_assign_cue(AssignmentStatus& status) {
		const bool b1 = maybe_assign_cue_source(status);
		const bool b2 = maybe_assign_cue_target(status);
		return b1 || b2;
	}
}

#endif //IDEA_CUE_H
