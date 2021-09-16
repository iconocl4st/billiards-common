//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_TABLELAYOUT_H
#define GLVIEW_TABLELAYOUT_H

#include "../config/BallDimensions.h"
#include "../geometry/Point.h"
#include <list>

namespace billiards::layout {

	class LocatedBall {
	public:
		config::BallInfo info;
		geometry::Point location;

		LocatedBall(const config::BallInfo& info, const geometry::Point& location) :
			info{info},
			location{location} {}

		~LocatedBall() = default;

//		inline
//		virtual void to_json(SaxWriter& writer) override {
//			writer.begin_object();
//			writer.key("info");
//			info.to_json(writer);
//			writer.key("location");
//			location.to_json(writer);
//			writer.end_object();
//		}
//
//		inline
//		virtual void parse(const nlohmann::json& value) override {
//
//		}
	};

	class TableLayout {
	public:
		std::list<LocatedBall> balls;
	};

}


#endif //GLVIEW_TABLELAYOUT_H
