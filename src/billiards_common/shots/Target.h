//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_TARGET_H
#define IDEA_TARGET_H

namespace billiards::shots {
	namespace target_type {
		enum TargetType {
			GOAL_POST,
			// More coming soon?
			UNKNOWN,
		};

		[[nodiscard]] inline
		std::string to_string(const TargetType type) {
			switch (type) {
				case GOAL_POST:
					return "goal-posts";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}

		[[nodiscard]] inline
		TargetType from_string(const std::string& str) {
			if (str == "goal-posts") {
				return GOAL_POST;
			} else {
				return UNKNOWN;
			}
		}
	}

	class Target : public billiards::json::Serializable {
	public:
		target_type::TargetType type;

		Target(target_type::TargetType type) : type{type} {}

		virtual ~Target() = default;
	};

	class GoalPostTarget : public Target {
	public:
		// TODO:
		// Maybe this should have been an array of length 3...
		geometry::MaybePoint goal_post_1;
		geometry::MaybePoint goal_post_2;
		geometry::MaybePoint goal_post_center;

		GoalPostTarget()
			: Target{target_type::GOAL_POST}, goal_post_1{}, goal_post_2{}, goal_post_center{} {}

		~GoalPostTarget() override = default;

		std::array<geometry::MaybePoint, 3> posts() const {
			return std::array<geometry::MaybePoint, 3> {
				goal_post_1,
				goal_post_2,
				goal_post_2
			};
		}

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("type", target_type::to_string(type));
			writer.key("goal-post-1");
			goal_post_1.to_json(writer);
			writer.key("goal-post-2");
			goal_post_2.to_json(writer);
			writer.key("goal-post-center");
			goal_post_center.to_json(writer);
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_STRING(status, value, "type", "Goal posts must have a type");
			if (type != target_type::from_string(value["type"].get<std::string>())) {
				status.success = false;
				status.error_msg << "Trying to parse the wrong type of target";
				return;
			}
			REQUIRE_CHILD(status, value, "goal-post-1", goal_post_1, "Missing goal post");
			REQUIRE_CHILD(status, value, "goal-post-2", goal_post_2, "Missing goal post");
			REQUIRE_CHILD(status, value, "goal-post-center", goal_post_center, "Missing goal post");
		}
	};

	namespace targets {
		std::shared_ptr<Target> parse(const nlohmann::json& value, json::ParseResult& status) {
			if (!HAS_STRING(value, "type")) {
				status.success = false;
				status.error_msg << "Missing type";
				return nullptr;
			}
			auto type = target_type::from_string(value["type"].get<std::string>());
			std::shared_ptr <Target> ret;
			switch (type) {
				case target_type::GOAL_POST: {
					auto p = std::make_shared<GoalPostTarget>();
					ret = std::dynamic_pointer_cast<Target>(p);
					break;
				}
				case target_type::UNKNOWN:
				default:
					status.success = false;
					status.error_msg << "Unknown type";
					return nullptr;
			}
			ret->parse(value, status);
			return ret;
		}
	}
}
#endif //IDEA_TARGET_H
