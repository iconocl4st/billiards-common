//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_UTIL_H
#define IDEA_UTIL_H

#include "ShotInfoParams.h"
#include "billiards_common/shots/ShotInformation.h"
#include "billiards_common/shots/shot_helpers.h"

namespace billiards::shots {

	class Assignment;

	class AssignmentStatus {
	public:
		Assignment& ass;
		const shots::ShotInfoParams& params;
		shots::ShotInformation& info;

		bool assigned_exiting_loc;
		geometry::MaybePoint exiting_location;
		bool assigned_exiting_radius;
		geometry::MaybeDouble exiting_radius;
		bool assigned_target;

		int index;

		AssignmentStatus(Assignment& ass, const shots::ShotInfoParams& params, shots::ShotInformation& info, int index)
			: ass{ass}
			, params{params}
			, info{info}
			, assigned_exiting_loc{false}
			, assigned_exiting_radius{false}
			, assigned_target{false}
			, index{index}
		{}

		~AssignmentStatus() = default;

		[[nodiscard]] inline
		StepInfo& get_info() {
			return info.get_info(index);
		}

		[[nodiscard]] inline
		const StepInfo& get_info() const {
			return info.get_info(index);
		}

		[[nodiscard]] inline
		std::shared_ptr<GoalPostTarget>& get_target() {
			return info.get_info(index).target;
		}

		[[nodiscard]] inline
		const std::shared_ptr<GoalPostTarget>& get_target() const {
			return info.get_info(index).target;
		}

		[[nodiscard]] inline
		bool already_has_source() const {
			return assigned_exiting_loc && assigned_exiting_radius;
		}

		[[nodiscard]] inline
		bool assigned() const {
			return already_has_source() && assigned_target;
		}

		[[nodiscard]] inline
		bool already_has_target() const {
			return assigned_target;
		}

		inline
		void no_source() {
			assigned_exiting_radius = true;
			assigned_exiting_loc = true;
		}

		inline
		void no_target() {
			assigned_target = true;
		}

		inline
		void assign_target(std::shared_ptr<shots::GoalPostTarget>& target) {
			info.get_info(index).target = target;
			assigned_target = true;
			for (int i = 0; i < 3; i++) {
				info.update_validity(target->posts[i].is_valid());
			}
		}

		inline
		void assign_exiting_location(const geometry::MaybePoint& p) {
			exiting_location = p;
			assigned_exiting_loc = true;
			info.update_validity(p.is_valid());
		}

		inline
		void assign_exiting_radius(const geometry::MaybeDouble& r) {
			exiting_radius = r;
			assigned_exiting_radius = true;
			info.update_validity(r.is_valid());
		}

		inline
		void assign_exiting(int ball_index) {
			assign_exiting_location(params.locations.balls[ball_index].location);
			assign_exiting_radius(get_ball_type(params.config, params.locations, ball_index)->radius);
		}

		friend std::ostream& operator<<(std::ostream& out, const AssignmentStatus& status) {
			out << "<t=" << status.assigned_target << ",";
			out << "r=" << status.assigned_exiting_radius << ",";
			out << "l=" << status.assigned_exiting_loc << ">";
			return out;
		}

//		[[nodiscard]] inline
//		bool update(const AssignmentStatus& other) {
//			bool prev_assigned_source = assigned_source;
//			bool prev_assigned_target = assigned_target;
//			assigned_source |= other.assigned_source;
//			assigned_target |= other.assigned_target;
//			return (
//				prev_assigned_source != other.assigned_source ||
//				prev_assigned_target != other.assigned_target);
//		}
	};

	class Assignment {
	public:
		std::vector<AssignmentStatus> statuses;
		const shots::ShotInfoParams& params;
		shots::ShotInformation& info;

		Assignment(const shots::ShotInfoParams& params, shots::ShotInformation& info, int num_steps)
			: statuses{}
			, params{params}
			, info{info}
		{
			for (int i = 0; i < num_steps; i++) {
				statuses.emplace_back(*this, params, info, i);
			}
		}

		/*
		[[nodiscard]] inline
		bool has_source_for(int index) const {
			return index > 0 && statuses[index - 1].assigned_source;
		}

		[[nodiscard]] inline
		bool has_target_for(int index) const {
			return index < statuses.size() - 1 && statuses[index + 1].assigned_target;
		}
		 */

		friend std::ostream& operator<<(std::ostream& out, const Assignment& status) {
			out << "[Assignment:";
			for (const auto& status : status.statuses) {
				out << status;
			}
			return out;
		}
	};

	[[nodiscard]] inline
	AssignmentStatus& get_next_status(AssignmentStatus& status) {
		return status.ass.statuses[status.index + 1];
	}

	[[nodiscard]] inline
	AssignmentStatus& get_previous_status(AssignmentStatus& status) {
		return status.ass.statuses[status.index - 1];
	}

//	bool has_source(
//		const
//	)
//	bool has_source = false;
//	const geometry::MaybePoint source;
//	if (index > 0 && assigned[index - 1].assigned_source) {
//	source = info.destinations[index - 1].exiting_source;
//	has_source = true;
//}
//bool has_target = false;
//const std::shared_ptr<GoalPostTarget> target;
//if (index < info.destinations.size() - 1 && assigned[index + 1].assigned_target) {
//target = info.destinations[index + 1].target;
//has_target = true;
//}
}

#endif //IDEA_UTIL_H
