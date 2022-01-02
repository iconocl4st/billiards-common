//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_LAYOUTSHOT_H
#define IDEA_LAYOUTSHOT_H

#include "billiards_common/shots/Shot.h"

namespace billiards::layout {

	class LayoutShot : public billiards::json::Serializable {
	public:
		bool active;
		shots::Shot shot;

		LayoutShot(shots::Shot shot)
			: active{true}
			, shot{shot}
		{}

		LayoutShot() : LayoutShot{shots::Shot{}} {}

		~LayoutShot() override = default;

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_BOOL(status, value, "active", "The shot must indicate if it is active");
			active = value["active"].get<bool>();
			REQUIRE_CHILD(status, value, "shot", shot, "Must have a shot");
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("active", active);
			writer.key("shot");
			shot.to_json(writer);
			writer.end_object();
		}
	};
}

#endif //IDEA_LAYOUTSHOT_H
