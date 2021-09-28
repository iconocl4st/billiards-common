//
// Created by thallock on 9/27/21.
//

#ifndef IDEA_LOCATEDBALL_H
#define IDEA_LOCATEDBALL_H

#include "./VirtualBall.h"
#include "../geometry/Point.h"

namespace billiards::layout {

	class LocatedBall : public json::Serializable {
	public:
		vball::VirtualBall ball;
		geometry::Point location;

		LocatedBall(const vball::VirtualBall& ball, const geometry::Point& location) :
			ball{ball},
			location{location} {}
		LocatedBall() : ball{}, location{} {}
		~LocatedBall() = default;

		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("ball");
			ball.to_json(writer);
			writer.key("location");
			location.to_json(writer);
			writer.end_object();
		}

		inline
		void parse(const nlohmann::json& value) override {
			if (value.contains("ball") && value["ball"].is_object()) {
				ball.parse(value["ball"]);
			}
			if (value.contains("location") && value["location"].is_object()) {
				location.parse(value["location"]);
			}
		}
	};
}


#endif //IDEA_LOCATEDBALL_H
