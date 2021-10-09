//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_SHOTSTEP_H
#define IDEA_SHOTSTEP_H

#include <memory>

#include "billiards_common/utils/Serializable.h"

namespace billiards::shots {

	namespace step_type {
		enum ShotStepType {
			CUE,
			STRIKE,
			RAIL,
			KISS,
			POCKET,
			UNKNOWN,

			// masse, jump
		};

		std::string to_string(const ShotStepType type) {
			switch (type) {
				case CUE:
					return "cue";
				case STRIKE:
					return "strike";
				case RAIL:
					return "rail";
				case KISS:
					return "kiss";
				case POCKET:
					return "pocket";
				default:
					return "unknown";
			}
		}

		ShotStepType from_string(const std::string& str) {
			if (str == "cue") {
				return CUE;
			} else if (str == "strike") {
				return STRIKE;
			} else if (str == "rail") {
				return RAIL;
			} else if (str == "kiss") {
				return KISS;
			} else if (str == "pocket") {
				return POCKET;
			} else {
				return UNKNOWN;
			}
		}
	}

	class ShotStep : public json::Serializable {
	public:
		step_type::ShotStepType type;

		ShotStep(step_type::ShotStepType type) : type{type} {}
		virtual ~ShotStep() = default;

		virtual void write_details(json::SaxWriter& writer) const = 0;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("type", step_type::to_string(type));
			write_details(writer);
			writer.end_object();
		}
	};

	class PocketStep : public ShotStep {
	public:
		// size_t?
		int pocket;

		PocketStep(int pocket)
			: ShotStep{step_type::POCKET}
			, pocket{pocket}
			{}

		PocketStep() : PocketStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("pocket", pocket);
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_NUMBER(status, value, "pocket", "must have a pocket");
			pocket = value["pocket"].get<int>();
		}
	};

	namespace kt {
		enum kiss_type {
			STUN,
			ROLLING,
			UNKNOWN,
		};

		[[nodiscard]] inline
		std::string to_string(kiss_type type) {
			switch (type) {
				case STUN:
					return "stun";
				case ROLLING:
					return "rolling";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}

		[[nodiscard]] inline
		kiss_type from_string(const std::string& type) {
			if (type == "stun") {
				return kiss_type::STUN;
			} else if (type == "rolling") {
				return kiss_type::ROLLING;
			} else {
				return kiss_type::UNKNOWN;
			}
		}

	}

	class KissStep : public ShotStep {
	public:
		int kissed_ball;
		kt::kiss_type type;

		KissStep(int kissed, kt::kiss_type type)
			: ShotStep{step_type::KISS}
			, type{type}
			, kissed_ball{kissed}
		{}

		KissStep() : KissStep{-1, kt::ROLLING} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("kissed-ball", kissed_ball);
			writer.field("kiss-type", kt::to_string(type));
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_NUMBER(status, value, "kissed-ball", "must have a kissed ball");
			// must be positive...
			kissed_ball = value["kissed-ball"].get<int>();
			ENSURE_STRING(status, value, "kiss-type", "must have a kiss type");
			type = kt::from_string(value["kiss-type"]);
		}
	};


	class CueStep : public ShotStep {
	public:
		int cue_ball;

		CueStep(int cue_ball)
			: ShotStep{step_type::CUE}
			,  cue_ball{cue_ball}
		{}

		CueStep() : CueStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("cue-ball", cue_ball);
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_NUMBER(status, value, "cue-ball", "must have a cue ball");
			cue_ball = value["cue-ball"].get<int>();
		}
	};


	class StrikeStep : public ShotStep {
	public:
		int object_ball;

		StrikeStep(int object_ball)
			: ShotStep{step_type::STRIKE}
			, object_ball{object_ball}
		{}

		StrikeStep() : StrikeStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("object-ball", object_ball);
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_NUMBER(status, value, "object-ball", "must have an object ball");
			object_ball = value["object-ball"].get<int>();
		}
	};


	class RailStep : public ShotStep {
	public:
		int rail;

		RailStep(int rail)
			: ShotStep{step_type::RAIL}
			, rail{rail}
		{}

		RailStep() : RailStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("rail", rail);
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_NUMBER(status, value, "rail", "must have a rail");
			rail = value["rail"].get<int>();
		}
	};

	namespace step {
		[[nodiscard]] inline
		ShotStep *from_type(step_type::ShotStepType type) {
			switch (type) {
				case step_type::CUE:
					return new CueStep;
				case step_type::STRIKE:
					return new StrikeStep;
				case step_type::RAIL:
					return new RailStep;
				case step_type::KISS:
					return new KissStep;
				case step_type::POCKET:
					return new PocketStep;
				case step_type::UNKNOWN:
				default:
					return nullptr;
			}
		}

		[[nodiscard]] inline
		std::shared_ptr<ShotStep> parse(const nlohmann::json& value, json::ParseResult& status) {
			if (!HAS_STRING(value, "type")) {
				status.success = false;
				status.error_msg << "the shot step must have a type";
				return nullptr;
			}

			step_type::ShotStepType type = step_type::from_string(value["type"].get<std::string>());
			ShotStep *ret = from_type(type);
			if (ret != nullptr) {
				ret->parse(value, status);
			}
			return std::shared_ptr<ShotStep>{ret};
		}
	}
}



#endif //IDEA_SHOTSTEP_H
