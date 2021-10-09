//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SHOTSTEPINFO_H
#define IDEA_SHOTSTEPINFO_H

#include "billiards_common/shots/StepDestination.h"

namespace billiards::shots {

	class CueingInfo : public billiards::json::Serializable {
	public:
		geometry::MaybeDouble precision;
		geometry::Point cue_location;
		geometry::MaybeDouble cue_radius;
		std::shared_ptr<std::array<double, 3>> cuts;

		CueingInfo() = default;
		~CueingInfo() override = default;

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("cue-location");
			cue_location.to_json(writer);
			writer.field("cue-radius", cue_radius.get());
			if (precision.is_valid()) {
				writer.field("precision", precision.get());
			}
			if (cuts) {
				writer.key("cuts");
				writer.begin_array();
				for (int i = 0; i < 3; i++) {
					writer.value(cuts->at(i));
				}
				writer.end_array();
			}
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			REQUIRE_CHILD(status, value, "cue-location", cue_location, "Cue info have a cue location");
			ENSURE_NUMBER(status, value, "cue-radius", "Cue info must have a cue radius");
			cue_radius = value["cue-radius"].get<double>();
			if (HAS_NUMBER(value, "precision")) {
				precision = value["precision"].get<double>();
			}
			if (HAS_ARRAY(value, "cuts")) {
				cuts = std::make_shared<std::array<double, 3>>();
				if (value["cuts"].size() != 3) {
					status.success = false;
					status.error_msg << "Expected 3 cuts, found: " << value["cuts"].size();
					return;
				}
				for (int i = 0; i < 3; i++) {
					cuts->at(i) = value["cuts"][i].get<double>();
				}
			}
		}
	};
}
#endif //IDEA_SHOTSTEPINFO_H
