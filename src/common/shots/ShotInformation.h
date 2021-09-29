//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOTINFORMATION_H
#define GLVIEW_SHOTINFORMATION_H

#include "./Shot.h"
#include "./ShotStepInfo.h"

namespace billiards::shots {

	class ShotInformation : public json::Serializable {
	public:
		Shot shot;
		CueingInfo cueing;
		std::vector<std::shared_ptr<Destination>> destinations;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("shot");
			shot.to_json(writer);
			writer.key("cueing");
			cueing.to_json(writer);
			writer.key("destinations");
			writer.begin_array();
			for (const auto& dest : destinations) {
				dest->to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("shot") && value["shot"].is_object()) {
				shot.parse(value["shot"]);
			}
			if (value.contains("cueing") && value["cueing"].is_object()) {
				cueing.parse(value["cueing"]);
			}
			if (value.contains("destinations") && value["destinations"].is_array()) {
				destinations.clear();
				for (const auto& it : value["destinations"]) {
					destinations.emplace_back(destinations::parse(it));
				}
			}

//			// This should have been cleaner, maybe the should only have an index?
//			for (int i = 0; i < destinations.size(); i++) {
//				auto dest = destinations[i];
//				auto step = shot.steps[i+1];
//				switch (dest->type) {
//					case dest_type::GHOST_BALL: {
//						auto d = std::dynamic_pointer_cast<GhostBallDestination>(dest);
//						auto s = std::dynamic_pointer_cast<StrikeStep>(step);
//						d->step = s;
//					}
//					case dest_type::POCKET: {
//						auto d = std::dynamic_pointer_cast<PocketDestination>(dest);
//						auto s = std::dynamic_pointer_cast<PocketStep>(step);
//						d->step = s;
//					}
//					case dest_type::UNKNOWN:
//					default:
//						throw std::runtime_error{"Unknown type"};
//				}
//			}
		}

		[[nodiscard]] bool is_possible() const {
			// TODO:
			return true;
		}
	};
}


#endif //GLVIEW_SHOTINFORMATION_H
