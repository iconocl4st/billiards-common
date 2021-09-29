//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SHOTSTEPINFO_H
#define IDEA_SHOTSTEPINFO_H

#include "common/geometry/Maybe.h"
#include <memory>

namespace billiards::shots {
	/*
	 * Still thinking through the structure here...
	 */

	namespace target_type {
		enum TargetType {
			GOAL_POST,
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
			: type{type}
		{}
		virtual ~Target() = default;
	};

	class GoalPostTarget : public Target {
	public:
		geometry::MaybePoint goal_post_1;
		geometry::MaybePoint goal_post_2;

		GoalPostTarget()
			: Target{target_type::GOAL_POST}
			, goal_post_1{}
			, goal_post_2{}
		{}
		~GoalPostTarget() override = default;

		void parse(const nlohmann::json& value) override {
			if (!value.contains("type")
				|| !value["type"].is_string()
				|| type != target_type::from_string(value["type"].get<std::string>())) {
				throw std::runtime_error{"Trying to parse the wrong type of target"};
			}
			if (value.contains("goal-post-1") && value["goal-post-1"].is_object()) {
				goal_post_1.parse(value["goal-post-1"]);
			}
			if (value.contains("goal-post-2") && value["goal-post-2"].is_object()) {
				goal_post_2.parse(value["goal-post-2"]);
			}
		}
		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("type", target_type::to_string(type));
			writer.key("goal-post-1");
			goal_post_1.to_json(writer);
			writer.key("goal-post-2");
			goal_post_2.to_json(writer);
			writer.end_object();
		}
	};

	/*
	 *  Maybe I don't need destinations...
	 */
	namespace dest_type {
		enum DestinationType {
			POCKET,
			GHOST_BALL, // Should be strike, I guess
			UNKNOWN,
		};

		std::string to_string(const DestinationType type) {
			switch (type) {
				case POCKET:
					return "pocket";
				case GHOST_BALL:
					return "ghost-ball";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}

		DestinationType from_string(const std::string& str) {
			if (str == "pocket") {
				return POCKET;
			} else if (str == "ghost-ball") {
				return GHOST_BALL;
			} else {
				return UNKNOWN;
			}
		}
	}

	class Destination : public billiards::json::Serializable {
	public:
		dest_type::DestinationType type;

		Destination(dest_type::DestinationType type)
			: type{type}
		{}
		virtual ~Destination() = default;

		virtual Target* get_target() = 0;
	};


	class PocketDestination : public Destination {
	public:
		int step_index;
		GoalPostTarget target;

		PocketDestination()
			: Destination{dest_type::GHOST_BALL}
			, step_index{-1}
			, target{}
		{}

		PocketDestination(int step_index)
			: Destination{dest_type::POCKET}
			, step_index{step_index}
			, target{}
		{}
		~PocketDestination() override = default;

		Target* get_target() override {
			return (Target *) &target;
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("target") && value["target"].is_object()) {
				target.parse(value["target"]);
			}
			if (value.contains("step-index") && value["step-index"].is_number()) {
				step_index = value["step-index"].get<int>();
			}
		}

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("type", dest_type::to_string(type));
			writer.field("step-index", step_index);
			writer.key("target");
			target.to_json(writer);
			writer.end_object();
		}
	};

	class GhostBallDestination : public Destination {
	public:
		int step_index;
		GoalPostTarget target;

		GhostBallDestination()
			: Destination{dest_type::GHOST_BALL}
			, step_index{-1}
			, target{}
		{}

		GhostBallDestination(int step_index)
			: Destination{dest_type::GHOST_BALL}
			, step_index{step_index}
			, target{}
		{}
		~GhostBallDestination() override = default;

		Target* get_target() override {
			return (Target *) &target;
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("target") && value["target"].is_object()) {
				target.parse(value["target"]);
			}
			if (value.contains("step-index") && value["step-index"].is_number()) {
				step_index = value["step-index"].get<int>();
			}
		}

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("type", dest_type::to_string(type));
			writer.field("step-index", step_index);
			writer.key("target");
			target.to_json(writer);
			writer.end_object();
		}
	};

	namespace destinations {
		std::shared_ptr<Destination> parse(const nlohmann::json& value) {
			if (!value.contains("type") || !value["type"].is_string()) {
				throw std::runtime_error{"Missing type"};
			}
			auto type = dest_type::from_string(value["type"].get<std::string>());
			std::shared_ptr<Destination> ret;
			switch (type) {
				case dest_type::POCKET: {
					auto p = std::make_shared<PocketDestination>();
					ret = std::dynamic_pointer_cast<Destination>(p);
					break;
				}
				case dest_type::GHOST_BALL: {
					auto p = std::make_shared<GhostBallDestination>();
					ret = std::dynamic_pointer_cast<Destination>(p);
					break;
				}
				case dest_type::UNKNOWN:
				default:
					throw std::runtime_error{"Unknown type"};
			}
			ret->parse(value);
			return ret;
		}
	}

	class CueingInfo : public billiards::json::Serializable {
	public:
		geometry::MaybeDouble lower_cut_angle;
		geometry::MaybeDouble upper_cut_angle;
		geometry::MaybeDouble precision;

		CueingInfo() = default;
		~CueingInfo() override = default;

		void parse(const nlohmann::json& value) override {
			if (value.contains("lower-cut-angle") && value["lower-cut-angle"].is_number()) {
				lower_cut_angle = value["lower-cut-angle"].get<double>();
			}
			if (value.contains("upper-cut-angle") && value["upper-cut-angle"].is_number()) {
				upper_cut_angle = value["upper-cut-angle"].get<double>();
			}
			if (value.contains("precision") && value["precision"].is_number()) {
				precision = value["precision"].get<double>();
			}
		}

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("lower-cut-angle", lower_cut_angle.get());
			writer.field("upper-cut-angle", upper_cut_angle.get());
			writer.field("precision", precision.get());
			writer.end_object();
		}
	};
}
#endif //IDEA_SHOTSTEPINFO_H
