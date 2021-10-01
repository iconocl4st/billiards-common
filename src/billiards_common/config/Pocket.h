//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_POCKET_H
#define GLVIEW_POCKET_H

#include "billiards_common/geometry/geometry.h"

namespace billiards::config {

	namespace horz_loc {
		enum HorizontalLocation {
			LEFT,
			MIDDLE,
			RIGHT,
			UNKNOWN,
		};

		[[nodiscard]] inline
		std::string to_string(HorizontalLocation h) {
			switch (h) {
				case RIGHT:
					return "right";
				case LEFT:
					return "left";
				case MIDDLE:
					return "middle";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}
	}
	namespace vert_loc {
		enum VerticalLocation {
			LOWER,
			UPPER,
			UNKNOWN,
		};

		[[nodiscard]] inline
		std::string to_string(VerticalLocation h) {
			switch (h) {
				case LOWER:
					return "right";
				case UPPER:
					return "left";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}
	}
	class PocketOrientation {
	public:
		horz_loc::HorizontalLocation horizontal;
		vert_loc::VerticalLocation vertical;

		PocketOrientation(horz_loc::HorizontalLocation h, vert_loc::VerticalLocation v)
			: horizontal{h}
			, vertical{v}
			{}

		PocketOrientation() : PocketOrientation{horz_loc::UNKNOWN, vert_loc::UNKNOWN} {}
		~PocketOrientation() = default;

		[[nodiscard]] inline
		std::string to_string() const {
			std::stringstream ss{};
			ss << horz_loc::to_string(horizontal) << " " << vert_loc::to_string(vertical);
			return ss.str();
		}
	};

	class Pocket : public json::Serializable {
	public:
		geometry::Point inner_segment_1;
		geometry::Point outer_segment_1;
		geometry::Point outer_segment_2;
		geometry::Point hole_center;

		PocketOrientation orientation;

		Pocket() = default;
		Pocket(geometry::Point outer_segment_1, geometry::Point inner_segment_1, geometry::Point outer_segment_2)
			: inner_segment_1{inner_segment_1}
			, outer_segment_1{outer_segment_1}
			, outer_segment_2{outer_segment_2}
			, hole_center{}
			, orientation{}
		{}
		~Pocket() = default;

		[[nodiscard]] geometry::Point center() const {
			// TODO: remove this method...
			return (outer_segment_1 + inner_segment_1 + outer_segment_2 + inner_segment_2().point()) / 4.0;
		}

		[[nodiscard]] inline
		geometry::MaybePoint inner_segment_2() const {
			auto segment = geometry::through(outer_segment_1, outer_segment_2);
			auto normal = geometry::orthogonal_at(segment, (outer_segment_1 + outer_segment_2) / 2);
			return geometry::reflect(inner_segment_1, normal);
		}

		void to_json(json::SaxWriter& writer) const {
			writer.begin_object();
			writer.key("outer-segment-1");
			outer_segment_1.to_json(writer);
			writer.key("outer-segment-2");
			outer_segment_2.to_json(writer);
			writer.key("inner-segment-1");
			inner_segment_1.to_json(writer);
			writer.end_object();
		};

		void parse(const nlohmann::json& value, json::ParseResult& status) {
			REQUIRE_CHILD(status, value, "inner-segment-1", inner_segment_1, "Missing an outer segment");
			REQUIRE_CHILD(status, value, "outer-segment-1", outer_segment_1, "Missing an outer segment");
			REQUIRE_CHILD(status, value, "outer-segment-2", outer_segment_2, "Missing an outer segment");
		};
	};
}

/*

import numpy as np
import matplotlib.pyplot as plt
a = np.array([
 # Upper right
	[2653, 36,], 		# outer segment 1
	[2706, 0],          # inner segment 1
	[2753, 125],        # outer segment 2
 # Upper middle
	[1469, 42],         # outer segment 1
	[1450, 0],          # inner segment 1
	[1292, 41],         # outer segment 2
 # Upper left
	[107, 38],          # outer segment 1
	[46, 0],            # inner segment 1
	[0, 128],           # outer segment 2
 # lower left
	[112, 1356],        # outer segment 1
	[82, 1380],         # inner segment 1
	[0, 1260],          # outer segment 2
 # lower middle
	[1300, 1359],       # outer segment 1
	[1303, 1380],       # inner segment 1
	[1468, 1358],       # outer segment 2
 # lower right
	[2650, 1352],       # outer segment 1
	[2698, 1380],       # inner segment 1
	[2755, 1250],       # outer segment 2
 ], dtype=np.float64)
a = np.array([92, 46]) * (a + 50) / np.max(a, axis=0)

plt.scatter(x=a[:,0], y=a[:,1])
plt.show()

*/
#endif //GLVIEW_POCKET_H
