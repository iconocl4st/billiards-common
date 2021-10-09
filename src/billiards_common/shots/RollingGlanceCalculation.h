//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_ROLLINGGLANCECALCULATION_H
#define IDEA_ROLLINGGLANCECALCULATION_H

#include <utility>

#include "billiards_common/geometry/Maybe.h"

namespace billiards::shots {

	class RollingGlanceCalculation : public billiards::json::Serializable {
	public:
		geometry::Point loc;
		geometry::Point tan_dir;
		geometry::Point aim_dir;

		RollingGlanceCalculation() = default;
		RollingGlanceCalculation(const RollingGlanceCalculation& o) = default;
		RollingGlanceCalculation(
			double x, double y,
			double tx, double ty,
			double ax, double ay
		) : loc{x, y}, tan_dir{tx, ty}, aim_dir{ax, ay} {}

		RollingGlanceCalculation(
			geometry::Point loc,
			geometry::Point tan,
			geometry::Point aim
		) : loc{std::move(loc)}, tan_dir{std::move(tan)}, aim_dir{std::move(aim)} {}

		~RollingGlanceCalculation() override = default;

		[[nodiscard]] inline
		RollingGlanceCalculation add_object(const geometry::Point& object_location) const {
			return RollingGlanceCalculation{
				loc + object_location,
				tan_dir,
				aim_dir
			};
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			REQUIRE_CHILD(status, value, "location", loc, "Must have a location");
			REQUIRE_CHILD(status, value, "tangent-direction", tan_dir, "Must have an aim direction");
			REQUIRE_CHILD(status, value, "aim-line-direction", aim_dir, "Must have a tangent direction");
		}

		void to_json(billiards::json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("location");
			loc.to_json(writer);
			writer.key("tangent-direction");
			tan_dir.to_json(writer);
			writer.key("aim-line-direction");
			aim_dir.to_json(writer);
			writer.end_object();
		}
	};
}

#endif //IDEA_ROLLINGGLANCECALCULATION_H
