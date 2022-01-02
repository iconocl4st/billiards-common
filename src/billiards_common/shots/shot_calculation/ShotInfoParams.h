//
// Created by thallock on 10/1/21.
//

#ifndef IDEA_SHOTINFOPARAMS_H
#define IDEA_SHOTINFOPARAMS_H

#include "billiards_common/config/PoolConfiguration.h"
#include "billiards_common/shots/Shot.h"
#include "billiards_common/shots/Locations.h"

namespace billiards::shots {

	class ShotInfoParams : public json::Serializable {
	public:
		config::PoolConfiguration config;
		layout::Locations locations;
		Shot shot;

		ShotInfoParams() = default;
		~ShotInfoParams() override = default;

		void to_json(json::SaxWriter& writer) const override {
			// params not written...
			writer.begin_object();
			writer.key("config");
			config.to_json(writer);
			writer.key("locations");
			locations.to_json(writer);
			writer.key("shot");
			shot.to_json(writer);
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			REQUIRE_CHILD(status, value, "config", config, "List parameters must have a config");
			REQUIRE_CHILD(status, value, "locations", locations, "List parameters must have locations");
			REQUIRE_CHILD(status, value, "shot", shot, "List parameters must a shot");
		}
	};
}

#endif //IDEA_SHOTINFOPARAMS_H
