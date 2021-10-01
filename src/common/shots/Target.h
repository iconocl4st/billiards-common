//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_TARGET_H
#define IDEA_TARGET_H

namespace billiards::shots {
	namespace target_type {
		enum TargetType {
			GOAL_POST,
			// More coming soon...
			UNKNOWN,
		};

		std::string to_string(const TargetType type) {
			switch (type) {
				case GOAL_POST:
					return "goal-posts";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}

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

		Target(target_type::TargetType type)
			: type{type} {}

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

		void parse(const nlohmann::json& value) override {
			if (!value.contains("type")
				|| !value["type"].is_string()
				|| type != target_type::from_string(value["type"].get<std::string>())) {
				throw std::runtime_error{"Trying to parse the wrong type of target"};
			}
			if (!value.contains("goal-post-1") || !value["goal-post-1"].is_object()) {
				throw std::runtime_error{"goal posts must have a first post"};
			}
			goal_post_1.parse(value["goal-post-1"]);

			if (!value.contains("goal-post-2") || !value["goal-post-2"].is_object()) {
				throw std::runtime_error{"goal posts must have a second post"};
			}
			goal_post_2.parse(value["goal-post-2"]);

			if (!value.contains("goal-post-center") || !value["goal-post-center"].is_object()) {
				throw std::runtime_error{"goal posts must have a center"};
			}
			goal_post_center.parse(value["goal-post-center"]);
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
	};

	namespace targets {
		std::shared_ptr <Target> parse(const nlohmann::json& value) {
			if (!value.contains("type") || !value["type"].is_string()) {
				throw std::runtime_error{"Missing type"};
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
					throw std::runtime_error{"Unknown type"};
			}
			ret->parse(value);
			return ret;
		}
	}
}
#endif //IDEA_TARGET_H
