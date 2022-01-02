//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_DETAILEDLAYOUT_H
#define IDEA_DETAILEDLAYOUT_H


#include "billiards_common/shots/ShotInformation.h"
#include "billiards_common/config/PoolConfiguration.h"
#include "billiards_common/layouts/Layout.h"

namespace billiards::layout {
	// Layout: assumes an ideal table based on a TableDimensions (width, height, pocket width)
	// Shots: converts an ideal layout and a pool config into a shot information...

	class DetailedLayout : public billiards::json::Serializable {
	public:
		config::PoolConfiguration config;
		Layout layout;
		std::vector<shots::ShotInformation> infos;

		DetailedLayout() = default;

		DetailedLayout(const config::PoolConfiguration config, const Layout& layout)
			: config{config}
			, layout{layout}
			, infos{}
		{
			for (const auto& shot : layout.shots) {
				infos.push_back(shots::ShotInformation{shot.shot});
			}
		}

		~DetailedLayout() override = default;

		void to_json(json::SaxWriter &writer) const override {
			writer.begin_object();
			writer.key("config");
			config.to_json(writer);
			writer.key("layout");
			layout.to_json(writer);
			writer.key("infos");
			writer.begin_array();
			for (const auto &info: infos) {
				info.to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json &value, json::ParseResult &status) override {
			REQUIRE_CHILD(status, value, "config", config, "Must have a table to render");
			REQUIRE_CHILD(status, value, "layout", layout, "Must have a layout");
			ENSURE_ARRAY(status, value, "infos", "Must have the shot infos");
			for (const auto &elem: value["infos"]) {
				infos.emplace_back();
				infos.back().parse(elem, status);
				if (!status.success) {
					return;
				}
			}
		}
	};
}


#endif //IDEA_DETAILEDLAYOUT_H
