//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_TABLELAYOUT_H
#define GLVIEW_TABLELAYOUT_H

#include <vector>

#include "./LocatedBall.h"
#include "../geometry/Dimensions.h"

namespace billiards::layout {

	class Locations : public json::Serializable {
	public:
		std::vector<LocatedBall> balls;
		geometry::Dimensions table_dims;

		Locations() = default;
		virtual ~Locations() = default;

		[[nodiscard]] int cue_ball_index() const {
			int index = 0;
			for (const auto& it : balls) {
				if (it.ball.is_cue()) {
					return index;
				}
				++index;
			}
			return -1;
		}
		
		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("balls");
			writer.begin_array();
			for (const auto& it : balls) {
				it.to_json(writer);
			}
			writer.end_array();
			writer.key("table-dimensions");
			table_dims.to_json(writer);
			writer.end_object();
		}

		inline
		void parse(const nlohmann::json& value) override {
			balls.clear();

			if (value.contains("balls") && value["balls"].is_array()) {
				for (const auto& it : value["balls"]) {
					balls.emplace_back(LocatedBall{});
					balls.back().parse(it);
				}
			}

			if (value.contains("table-dimensions") && value["table-dimensions"].is_object()) {
				table_dims.parse(value["table-dimensions"]);
			}
		}
	};

}


#endif //GLVIEW_TABLELAYOUT_H
