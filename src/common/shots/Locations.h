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

		[[nodiscard]] inline
		geometry::Point get_ball_location(int index) const {
			if (index < 0 || index >= balls.size()) {
				throw std::runtime_error{"Invalid index"};
			}
			return balls[index].location;
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
		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_ARRAY(status, value, "balls", "Locations must have balls");
			balls.clear();
			for (const auto& it: value["balls"]) {
				balls.emplace_back(LocatedBall{});
				PARSE_CHILD(status, it, balls.back());
			}
			REQUIRE_CHILD(status, value, "table-dimensions", table_dims, "Locations are specific to dimensions");
		}
	};

}


#endif //GLVIEW_TABLELAYOUT_H
