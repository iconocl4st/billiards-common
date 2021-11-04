//
// Created by thallock on 10/29/21.
//

#ifndef IDEA_LAYOUT_H
#define IDEA_LAYOUT_H

#include <list>

#include "billiards_common/shots/Locations.h"
#include "billiards_common/shots/Shot.h"
#include "billiards_common/graphics/parse_graphics.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


namespace billiards::layout : public billiards::json::Serializable {

    class Layout {
    public:
        boost::uuids::uuid uuid;

        Locations locations;
        std::list<shots::Shot> shots;
        std::list<std::shared_ptr<graphics::GraphicsPrimitive>> graphics;

        Layout() {}
        ~Layout() override = default;

        void parse(const nlohmann::json& value, json::ParseResult& status) override {
            ENSURE_STRING(status, value, "uuid", "Must have a uuid");
            uuid = boost::lexical_cast<uuid>(value["uuid"].get<std::string>());

            REQUIRE_CHILD(status, value, "locations", locations, "Must have a ball locations");


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
                auto g = parse_graphics(graphic, status);
                graphics.push_back(g);
                if (!status.success) {
                    return;
                }
            }
        }

        void to_json(json::SaxWriter& writer) const override {
            writer.begin_object();

            writer.key("uuid");
            writer.value(boost::lexical_cast<std::string>(uuid));

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
