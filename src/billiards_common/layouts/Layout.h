//
// Created by thallock on 10/29/21.
//

#ifndef IDEA_LAYOUT_H
#define IDEA_LAYOUT_H

#include <vector>

#include "billiards_common/shots/Locations.h"
#include "billiards_common/graphics/parse_graphics.h"
#include "billiards_common/layouts/LayoutShot.h"

namespace billiards::layout {

    class Layout : public billiards::json::Serializable {
    public:
		std::string name;
        Locations locations;
        std::vector<LayoutShot> shots;
        std::vector<std::shared_ptr<graphics::GraphicsPrimitive>> graphics;
		// Could have some wild cards as well...

        Layout()
            : name{"No name"}
			, locations{}
            , shots{}
            , graphics{}
        {}

        ~Layout() override = default;

        void parse(const nlohmann::json& value, json::ParseResult& status) override {
            REQUIRE_CHILD(status, value, "locations", locations, "Must have locations");

			ENSURE_STRING(status, value, "name", "Must have a name");
			name = value["name"].get<std::string>();

            ENSURE_ARRAY(status, value, "shots", "Must have a list of shots");
            shots.clear();
            for (const auto& shot : value["shots"]) {
                shots.emplace_back();
                shots.back().parse(shot, status);
                if (!status.success) {
                    return;
                }
            }

            ENSURE_ARRAY(status, value, "graphics", "Must have a list of graphics");
            graphics.clear();
            for (const auto& graphic : value["graphics"]) {
                auto g = graphics::parse_graphics(graphic, status);
                graphics.push_back(g);
                if (!status.success) {
                    return;
                }
            }
        }

        void to_json(json::SaxWriter& writer) const override {
            writer.begin_object();

			writer.field("name", name);

            writer.key("locations");
            locations.to_json(writer);

            writer.key("shots");
            writer.begin_array();
            for (const auto& shot : shots) {
                shot.to_json(writer);
            }
            writer.end_array();

            writer.key("graphics");
            writer.begin_array();
            for (const auto& g : graphics) {
                g->to_json(writer);
            }
            writer.end_array();

            writer.end_object();
        }
    };


}

#endif //IDEA_LAYOUT_H
