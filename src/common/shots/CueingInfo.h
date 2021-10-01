//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SHOTSTEPINFO_H
#define IDEA_SHOTSTEPINFO_H

#include "StepDestination.h"

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

		void parse(const nlohmann::json& value) override {
			if (value.contains("lower-cut-angle") && value["lower-cut-angle"].is_number()) {
				lower_cut_angle = value["lower-cut-angle"].get<double>();
			}
			if (value.contains("upper-cut-angle") && value["upper-cut-angle"].is_number()) {
				upper_cut_angle = value["upper-cut-angle"].get<double>();
			}
			if (!value.contains("precision") || !value["precision"].is_number()) {
				throw std::runtime_error{"Cueing info must have a precision"};
			}
			precision = value["precision"].get<double>();
			if (!value.contains("cue-location") || !value["cue-location"].is_object()) {
				throw std::runtime_error{"Cueing info must have a cue location"};
			}
			cue_location.parse(value["cue-location"]);
		}

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			if (lower_cut_angle.is_valid()) {
				writer.field("lower-cut-angle", lower_cut_angle.get());
			}
			if (upper_cut_angle.is_valid()) {
				writer.field("upper-cut-angle", upper_cut_angle.get());
			}
			std::cout << "Precision: " << precision.get() << std::endl;
			writer.field("precision", precision.get());
			writer.key("cue-location");
			cue_location.to_json(writer);
			writer.end_object();
		}
	};
}
#endif //IDEA_SHOTSTEPINFO_H
