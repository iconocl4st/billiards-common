//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SHOTSTEPINFO_H
#define IDEA_SHOTSTEPINFO_H

#include "billiards_common/shots/StepDestination.h"

namespace billiards::shots {

	class CueingInfo : public billiards::json::Serializable {
	public:
		geometry::MaybeDouble lower_cut_angle;
		geometry::MaybeDouble upper_cut_angle;

		// TODO: remove the maybes
		geometry::MaybeDouble precision;
		geometry::MaybePoint cue_location;

		CueingInfo() = default;
		~CueingInfo() override = default;

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			if (lower_cut_angle.is_valid()) {
				writer.field("lower-cut-angle", lower_cut_angle.get());
			}
			if (upper_cut_angle.is_valid()) {
				writer.field("upper-cut-angle", upper_cut_angle.get());
			}
			writer.field("precision", precision.get());
			writer.key("cue-location");
			cue_location.to_json(writer);
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			if (HAS_NUMBER(value, "lower-cut-angle")) {
				lower_cut_angle = value["lower-cut-angle"].get<double>();
			}
			if (HAS_NUMBER(value, "upper-cut-angle")) {
				upper_cut_angle = value["upper-cut-angle"].get<double>();
			}
			ENSURE_NUMBER(status, value, "precision", "Cueing info must have a precision");
			precision = value["precision"].get<double>();
			REQUIRE_CHILD(status, value, "cue-location", cue_location, "Cueing info must have a cue location");
		}
	};
}
#endif //IDEA_SHOTSTEPINFO_H
