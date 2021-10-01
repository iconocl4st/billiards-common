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

		[[nodiscard]] inline
		vball::virtual_type::VirtualBallType get_type() const {
			return ball.type;
		}

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
		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_OBJECT(status, value, "ball", "must have a ball");
			PARSE_CHILD(status, value["ball"], ball);
			ENSURE_OBJECT(status, value, "location", "must have a location");
			PARSE_CHILD(status, value["location"], location);
		}
	};
}


#endif //IDEA_LOCATEDBALL_H
