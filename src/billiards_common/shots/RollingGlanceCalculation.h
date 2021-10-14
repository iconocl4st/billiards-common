//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_ROLLINGGLANCECALCULATION_H
#define IDEA_ROLLINGGLANCECALCULATION_H

#include <utility>

#include "billiards_common/geometry/Maybe.h"

namespace billiards::shots {

#define CHECK_TOL (1e-5)

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
				tan_dir + object_location,
				aim_dir + object_location
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

		[[nodiscard]] inline
		bool is_feasible(const double dx, const double dy) const {
			const double x = loc.x;
			const double y = loc.y;
			const double tx = tan_dir.x;
			const double ty = tan_dir.y;
			const double aim_x = aim_dir.x;
			const double aim_y = aim_dir.y;

			const double alpha = 2 / 7.0;
			double t;
			if (std::abs(dx - x) >= TOLERANCE) {
				t = (alpha * (tx - x) + (1 - alpha) * (aim_x - x)) / (dx - x);
			} else if (std::abs(dy - y) >= TOLERANCE) {
				t = (alpha * (ty - y) + (1 - alpha) * (aim_y - y)) / (dy - y);
			} else {
				return false;
				throw std::runtime_error{"We already checked whether dx and dy are zero..."};
			}

			const double orthogonality_at_s = (aim_x - x) * (aim_x - tx) + (aim_y - y) * (aim_y - ty);
//			std::cout << "orthogonality: " << orthogonality_at_s << std::endl;
			if (std::abs(orthogonality_at_s) > CHECK_TOL) {
				return false;
			}

			const double rolling_x = alpha * (tx - x) + (1 - alpha) * (aim_x - x) - t * (dx - x);
//			std::cout << "rolling_x: " << rolling_x << std::endl;
			if (std::abs(rolling_x) > CHECK_TOL) {
				return false;
			}
			const double rolling_y = alpha * (ty - y) + (1 - alpha) * (aim_y - y) - t * (dy - y);
//			std::cout << "rolling_y: " << rolling_y << std::endl;
			if (std::abs(rolling_y) > CHECK_TOL) {
				return false;
			}

			const double t_same_direction = (tx - x) * (dx - x) + (ty - y) * (dy - y);
//			std::cout << "t same direction: " << t_same_direction << std::endl;
			if (t_same_direction < 0) {
				return false;
			}
			const double d_same_direction = (tx - x) * (dx - x) + (ty - y) * (dy - y);
//			std::cout << "d same direction as t: " << d_same_direction << std::endl;
			if (d_same_direction < 0) {
				return false;
			}
			const double a_same_direction = x * (aim_x - x) + y * (aim_y - y);
//			std::cout << "a same direction: " << a_same_direction << std::endl;
			if (a_same_direction < 0) {
				return false;
			}
			return true;
		}
	};
}

#endif //IDEA_ROLLINGGLANCECALCULATION_H
