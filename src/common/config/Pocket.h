//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_POCKET_H
#define GLVIEW_POCKET_H

#include "../geometry/geometry.h"

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

		std::string to_string() const {
			std::stringstream ss{};
			ss << horz_loc::to_string(horizontal) << " " << vert_loc::to_string(vertical);
			return ss.str();
		}
	};

	class Pocket : public json::Serializable {
	public:
		geometry::Point outerSegment1;
		geometry::Point outerSegment2;
		geometry::Point innerSegment1;
		geometry::Point holeCenter;

		PocketOrientation orientation;

		Pocket() = default;
		Pocket(geometry::Point outerSegment1, geometry::Point innerSegment1, geometry::Point outerSegment2)
			: outerSegment1{outerSegment1}
			, outerSegment2{outerSegment2}
			, innerSegment1{innerSegment1}
			, holeCenter{}
			, orientation{}
		{}
		~Pocket() = default;

		[[nodiscard]] geometry::Point center() const {
			// TODO: remove this method...
			return (outerSegment1 + outerSegment2 + innerSegment1 + innerSegment2().point()) / 4.0;
		}

		[[nodiscard]] inline
		geometry::MaybePoint innerSegment2() const {
			auto segment = geometry::through(outerSegment1, outerSegment2);
			auto normal = geometry::orthogonal_at(segment, (outerSegment1 + outerSegment2) / 2);
			return geometry::reflect(innerSegment1, normal);
		}

		void to_json(json::SaxWriter& writer) const {
			writer.begin_object();
			writer.key("outer-segment-1");
			outerSegment1.to_json(writer);
			writer.key("outer-segment-2");
			outerSegment2.to_json(writer);
			writer.key("inner-segment-1");
			innerSegment1.to_json(writer);
			writer.end_object();
		};

		void parse(const nlohmann::json& value) {
			if (value.contains("outer-segment-1") && value["outer-segment-1"].is_object()) {
				outerSegment1.parse(value["outer-segment-1"]);
			}
			if (value.contains("outer-segment-2") && value["outer-segment-2"].is_object()) {
				outerSegment2.parse(value["outer-segment-2"]);
			}
			if (value.contains("inner-segment-1") && value["inner-segment-1"].is_object()) {
				innerSegment1.parse(value["inner-segment-1"]);
			}
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
