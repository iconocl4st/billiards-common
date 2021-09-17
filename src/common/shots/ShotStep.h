//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_SHOTSTEP_H
#define IDEA_SHOTSTEP_H

#include <memory>

#include "common/utils/Serializable.h"

namespace billiards::shots {

	enum ShotStepType {
		CUE,
		STRIKE,
		RAIL,
		KISS,
		POCKET,
		UNKNOWN,

		// masse, jump
	};

	std::string step_type_to_string(const ShotStepType type) {
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

	ShotStepType string_to_step_type(const std::string& str) {
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

	class ShotStep : public json::Serializable {
	public:
		ShotStepType type;

		ShotStep(ShotStepType type) : type{type} {}
		virtual ~ShotStep() = default;

		virtual void write_details(json::SaxWriter& writer) const = 0;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("type", step_type_to_string(type));
			write_details(writer);
			writer.end_object();
		}
	};

	class PocketStep : public ShotStep {
	public:
		int pocket;

		PocketStep(int pocket)
			: ShotStep{POCKET}
			, pocket{pocket}
			{}

		PocketStep() : PocketStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("pocket", pocket);
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("pocket") && value["pocket"].is_number()) {
				pocket = value["pocket"].get<int>();
			} else {
				pocket = -1;
			}
		}
	};


	class KissStep : public ShotStep {
	public:
		int kissed_ball;

		KissStep(int kissed)
			: ShotStep{KISS}
			, kissed_ball{kissed}
		{}

		KissStep() : KissStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("kissed-ball", kissed_ball);
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("kissed-ball") && value["kissed-ball"].is_number()) {
				kissed_ball = value["kissed-ball"].get<int>();
			} else {
				kissed_ball = -1;
			}
		}
	};


	class CueStep : public ShotStep {
	public:
		int cue_ball;

		CueStep(int cue_ball)
			: ShotStep{CUE}
			,  cue_ball{cue_ball}
		{}

		CueStep() : CueStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("cue-ball", cue_ball);
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("cue-ball") && value["cue-ball"].is_number()) {
				cue_ball = value["cue-ball"].get<int>();
			} else {
				cue_ball = -1;
			}
		}
	};


	class StrikeStep : public ShotStep {
	public:
		int object_ball;

		StrikeStep(int object_ball)
			: ShotStep{STRIKE}
			, object_ball{object_ball}
		{}

		StrikeStep() : StrikeStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("object-ball", object_ball);
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("object-ball") && value["object-ball"].is_number()) {
				object_ball = value["object-ball"].get<int>();
			} else {
				object_ball = -1;
			}
		}
	};


	class RailStep : public ShotStep {
	public:
		int rail;

		RailStep(int rail)
			: ShotStep{RAIL}
			, rail{rail}
		{}

		RailStep() : RailStep{-1} {}

		void write_details(json::SaxWriter& writer) const override {
			writer.field("rail", rail);
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("rail") && value["rail"].is_number()) {
				rail = value["rail"].get<int>();
			} else {
				rail = -1;
			}
		}
	};

	namespace step {
		ShotStep *from_type(ShotStepType type) {
			switch (type) {
				case CUE:
					return new CueStep;
				case STRIKE:
					return new StrikeStep;
				case RAIL:
					return new RailStep;
				case KISS:
					return new KissStep;
				case POCKET:
					return new PocketStep;
				case UNKNOWN:
				default:
					return nullptr;
			}
		}

		std::shared_ptr<ShotStep> parse(const nlohmann::json& value) {
			if (!value.contains("type") || !value["type"].is_string()) {
				return nullptr;
			}

			ShotStepType type = string_to_step_type(value["type"].get<std::string>());
			ShotStep *ret = from_type(type);
			if (ret != nullptr) {
				ret->parse(value);
			}
			return std::shared_ptr<ShotStep>{ret};
		}
	}
}



#endif //IDEA_SHOTSTEP_H
