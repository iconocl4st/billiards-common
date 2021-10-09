//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_STEPWILDCARD_H
#define IDEA_STEPWILDCARD_H

#include "billiards_common/shots/ShotStep.h"
#include "billiards_common/shots/Locations.h"

namespace billiards::shots {

	namespace wild_card {
		enum wild_card_type {
			ANY,
			ONE_OF,
			EXACT,
			UNKNOWN,
		};

		[[nodiscard]] inline
		std::string to_string(wild_card_type type) {
			switch (type) {
				case ANY:
					return "any";
				case EXACT:
					return "exact";
				case ONE_OF:
					return "one-of";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}

		[[nodiscard]] inline
		wild_card_type from_string(const std::string& type) {
			if (type == "any") {
				return ANY;
			} else if (type == "exact") {
				return EXACT;
			} else if (type == "one-of") {
				return ONE_OF;
			} else {
				return UNKNOWN;
			}
		}
	}

	class ShotStepWildCard : public json::Serializable {
	public:
		step_type::ShotStepType step_type;
		wild_card::wild_card_type choices_type;
		int exact_value;
		std::list<int> possible_values;

		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("step-type", step_type::to_string(step_type));
			writer.field("choice-type", wild_card::to_string(choices_type));

			switch (choices_type) {
				case wild_card::ONE_OF:
					writer.key("values");
					writer.begin_array();
					for (const int& v : possible_values) {
						writer.value(v);
					}
					writer.end_array();
					break;
				case wild_card::EXACT:
					writer.field("value", exact_value);
					break;
				case wild_card::ANY:
				case wild_card::UNKNOWN:
					break;
			}
			writer.end_object();
		}

		inline
		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_STRING(status, value, "choice-type", "must have a choice type");
			choices_type = wild_card::from_string(value["choice-type"]);
			ENSURE_STRING(status, value, "step-type", "must have a step type");
			step_type = step_type::from_string(value["step-type"]);
			switch (choices_type) {
				case wild_card::EXACT: {
					ENSURE_NUMBER(status, value, "value", "If it is an exact value, it must have the value");
					exact_value = value["value"].get<int>();
				}
				case wild_card::ONE_OF: {
					ENSURE_ARRAY(status, value, "values", "If it is a one-of wild card, it must have the possible values");
					possible_values.clear();
					for (const auto& v : value["values"]) {
						possible_values.push_back(v.get<int>());
					}
				}
				case wild_card::ANY:
				case wild_card::UNKNOWN:
					break;
			}
		}
	};

//	class ExactValue : ShotStepWildCard {
//	public:
//		int value;
//	};
//
//	class Any : ShotStepWildCard {
//	public:
//	};
//
//	class OneOf : ShotStepWildCard {
//	public:
//		std::list<int> values;
//	};
}

#endif //IDEA_STEPWILDCARD_H
