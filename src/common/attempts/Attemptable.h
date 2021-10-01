//
// Created by thallock on 9/21/21.
//

#ifndef IDEA_ATTEMPTABLE_H
#define IDEA_ATTEMPTABLE_H

#include "../shots/Shot.h"
#include "../shots/Positions.h"

namespace billiards::attempts {

	class Attemptable : public json::Serializable {
	public:
		layout::Layout layout;
		shots::Shot shot;

		Attemptable() = default;
		virtual ~Attemptable() = default;

		[[nodiscard]] inline
		int compare(const Attemptable& other) const {
			int lcmp = layout.compare(other.layout);
			if (lcmp != 0) {
				return lcmp;
			}
			int scmp = shot.compare(other.shot);
			if (scmp != 0) {
				return scmp;
			}
			return 0;
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("layout");
			layout.to_json(writer);
			writer.key("shot");
			shot.to_json(writer);
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("layout") && value["layout"].is_object()) {
				layout.parse(value["layout"]);
			}
			if (value.contains("shot") && value["shot"].is_object()) {
				shot.parse(value["shot"]);
			}
		}
	};
}


#endif //IDEA_ATTEMPTABLE_H
