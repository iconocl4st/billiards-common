//
// Created by thallock on 12/26/21.
//

#ifndef IDEA_MAPPINGINFO_H
#define IDEA_MAPPINGINFO_H


#include "billiards_common/config/TableGeometry.h"

namespace billiards::project {

	class MappingInformation : public json::Serializable {
	public:
		geometry::Dimensions screen_size;
		config::TableGeometry physical;
		config::TableGeometry screen;

		MappingInformation()
			: screen_size{1920, 1080}
			, physical{config::TableDimensions{}}
			, screen{config::TableDimensions{screen_size.width, screen_size.height, 100}}
		{}

		~MappingInformation() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("screen-size");
			screen_size.to_json(writer);
			writer.key("physical");
			physical.to_json(writer);
			writer.key("screen");
			screen.to_json(writer);
			writer.end_object();
		}

		void parse([[maybe_unused]] const nlohmann::json &value, [[maybe_unused]] json::ParseResult &result) override {
			if (HAS_OBJECT(value, "physical")) {
				PARSE_CHILD(result, value["physical"], physical);
			}
			if (HAS_OBJECT(value, "screen")) {
				PARSE_CHILD(result, value["screen"], screen);
			}
			if (HAS_OBJECT(value, "screen-size")) {
				PARSE_CHILD(result, value["screen-size"], screen_size);
			}
		}
	};
}



#endif //IDEA_MAPPINGINFO_H
