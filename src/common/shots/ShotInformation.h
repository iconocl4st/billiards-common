//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOTINFORMATION_H
#define GLVIEW_SHOTINFORMATION_H

#include "./Shot.h"

namespace billiards::shots {

	class ShotInformation : public json::Serializable {
	public:
		Shot shot;
		// TODO: Fill in the missing calculated information...

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("shot");
			shot.to_json(writer);
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("shot") && value["shot"].is_object()) {
				shot.parse(value["shot"]);
			}
		}
	};

}


#endif //GLVIEW_SHOTINFORMATION_H
