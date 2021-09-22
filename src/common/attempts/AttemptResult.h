//
// Created by thallock on 9/21/21.
//

#ifndef IDEA_ATTEMPTRESULT_H
#define IDEA_ATTEMPTRESULT_H

#include "common/utils/Serializable.h"

namespace billiards::attempts {

	class AttemptResult : public json::Serializable  {
	public:
		uint64_t
		std::list<bool> successes;

		AttemptResult() = default;
		~AttemptResult() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_array();
			for (const auto& b : successes) {
				writer.value(b);
			}
			writer.end_array();
		}

		void parse(const nlohmann::json& value) override {
			if (!value.is_array()) {
				return;
			}
			for (const auto& v : value) {
				if (v.is_boolean()) {
					successes.push_back(v.get<bool>());
				}
			}
		}
	};


	namespace result {
		inline
		void assign_attempts(AttempResult& result, int num_attempts) {
			for (int i = 0; i < num_attempts - 1; i++) {
				result.successes.push_back(false);
			}
			result.success.push_back(true);
		}
	}
}
#endif //IDEA_ATTEMPTRESULT_H
