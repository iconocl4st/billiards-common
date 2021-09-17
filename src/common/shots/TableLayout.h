//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_TABLELAYOUT_H
#define GLVIEW_TABLELAYOUT_H

#include "../config/BallDimensions.h"
#include "../geometry/Point.h"
#include <list>

namespace billiards::layout {

	class LocatedBall : public json::Serializable {
	public:
		config::BallInfo info;
		geometry::Point location;

		LocatedBall(const config::BallInfo& info, const geometry::Point& location) :
			info{info},
			location{location} {}
			
		
		LocatedBall() : info{}, location{} {}

		~LocatedBall() = default;

		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("info");
			info.to_json(writer);
			writer.key("location");
			location.to_json(writer);
			writer.end_object();
		}

		inline
		void parse(const nlohmann::json& value) override {
			if (value.contains("info") && value["info"].is_object()) {
				info.parse(value["info"]);
			}
			if (value.contains("location") && value["location"].is_object()) {
				location.parse(value["location"]);
			}
		}
	};

	class TableLayout : public json::Serializable {
	public:
		std::list<LocatedBall> balls;
		
		TableLayout() = default;
		virtual ~TableLayout() = default;
		
		
		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("balls");
			for (const auto& it : balls) {
				it.to_json(writer);
			}
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
		}
	};

}


#endif //GLVIEW_TABLELAYOUT_H
