//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_TARGET_H
#define IDEA_TARGET_H

namespace billiards::shots {
//	namespace target_type {
//		enum TargetType {
//			GOAL_POST,
//			// More coming soon?
//			UNKNOWN,
//		};
//
//		[[nodiscard]] inline
//		std::string to_string(const TargetType type) {
//			switch (type) {
//				case GOAL_POST:
//					return "goal-posts";
//				case UNKNOWN:
//				default:
//					return "unknown";
//			}
//		}
//
//		[[nodiscard]] inline
//		TargetType from_string(const std::string& str) {
//			if (str == "goal-posts") {
//				return GOAL_POST;
//			} else {
//				return UNKNOWN;
//			}
//		}
//	}

//	class Target : public billiards::json::Serializable {
//	public:
//		target_type::TargetType type;
//
//		Target(target_type::TargetType type) : type{type} {}
//
//		virtual ~Target() = default;
//	};

	class GoalPostTarget : public json::Serializable {
	public:
		std::array<geometry::MaybePoint, 3> posts;

		GoalPostTarget() = default;
		~GoalPostTarget() override = default;

		inline
		void operator=(const geometry::Point& point) {
			for (int i = 0; i < 3; i++) {
				posts[i] = point;
			}
		}

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("posts");
			writer.begin_array();
			for (const auto& post : posts) {
				post.to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_ARRAY(status, value, "posts", "Must have a goal posts array");
			if (value["posts"].size() != 3) {
				///
			}
			for (int i = 0; i < 3; i++) {
				posts[i].parse(value["posts"][i]);
			}
		}
	};

//	namespace targets {
//		std::shared_ptr<Target> parse(const nlohmann::json& value, json::ParseResult& status) {
//			if (!HAS_STRING(value, "type")) {
//				status.success = false;
//				status.error_msg << "Missing type";
//				return nullptr;
//			}
//			auto type = target_type::from_string(value["type"].get<std::string>());
//			std::shared_ptr <Target> ret;
//			switch (type) {
//				case target_type::GOAL_POST: {
//					auto p = std::make_shared<GoalPostTarget>();
//					ret = std::dynamic_pointer_cast<Target>(p);
//					break;
//				}
//				case target_type::UNKNOWN:
//				default:
//					status.success = false;
//					status.error_msg << "Unknown type";
//					return nullptr;
//			}
//			ret->parse(value, status);
//			return ret;
//		}
//	}
}
#endif //IDEA_TARGET_H
