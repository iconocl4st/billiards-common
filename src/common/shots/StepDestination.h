//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_CUEINGINFO_H
#define IDEA_CUEINGINFO_H

#include <memory>

#include "common/geometry/Maybe.h"

#include "Target.h"

namespace billiards::shots {

		class Destination : public billiards::json::Serializable {
		public:
			int step_index;
			std::shared_ptr<Target> target;

			Destination() = default;
			Destination(int step_index) : step_index{step_index}, target{nullptr} {}
			virtual ~Destination() = default;

			void parse(const nlohmann::json& value) override {
				if (!value.contains("target") || !value["target"].is_object()) {
					throw std::runtime_error{"Destination must have a target"};
				}
				target = targets::parse(value["target"]);

				if (!value.contains("step-index") || !value["step-index"].is_number()) {
					throw std::runtime_error{"Destination must have a step index"};
				}
				step_index = value["step-index"].get<int>();
			}

			void to_json(billiards::json::SaxWriter& writer) const override {
				writer.begin_object();
				writer.field("step-index", step_index);
				writer.key("target");
				target->to_json(writer);
				writer.end_object();
			}
		};
}


#endif //IDEA_CUEINGINFO_H
