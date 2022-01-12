//
// Created by thallock on 12/26/21.
//

#ifndef IDEA_MAPPINGINFO_H
#define IDEA_MAPPINGINFO_H


#include "billiards_common/config/TableGeometry.h"

namespace billiards::project {

	[[nodiscard]] inline
	config::TableDimensions calculate_screen_dims(
		const geometry::Dimensions& screen_size,
		const config::TableDimensions& dims
	) {
		const double aspect = dims.height / dims.width;
		if (screen_size.width * aspect < screen_size.height) {
			return config::TableDimensions{
					screen_size.width,
					aspect * screen_size.width,
					dims.pocket_width * screen_size.width / dims.width
			};
		} else {
			return config::TableDimensions{
					screen_size.height / aspect,
					screen_size.height,
					dims.pocket_width * screen_size.height / dims.height
			};
		}
	}


	class MappingInformation : public json::Serializable {
	public:
		geometry::Dimensions screen_size;
		config::TableGeometry physical;
		config::TableGeometry screen;

		MappingInformation(
			const geometry::Dimensions screen_size,
			const config::TableDimensions& physical_dims
		)
				: screen_size{1920, 1080}
				, physical{physical_dims}
				, screen{calculate_screen_dims(screen_size, physical_dims)}
		{}

		MappingInformation()
			: MappingInformation(
				geometry::Dimensions{1920, 1080},
				config::TableDimensions{})
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
