//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_POCKET_H
#define GLVIEW_POCKET_H

#include "../geometry/geometry.h"

namespace billiards::config {

	class Pocket {
	public:
		geometry::Point outerSegment1;
		geometry::Point outerSegment2;
		geometry::Point innerSegment1;
		geometry::Point holeCenter;

		inline
		geometry::Point innerSegment2() {
			auto segment = geometry::through(outerSegment1, outerSegment2);
			auto normal = geometry::orthogonal_at(segment, (outerSegment1 + outerSegment2) / 2);
			auto refl = geometry::reflect(innerSegment1, normal);
			return refl.value_or(innerSegment1);
		}
	};
}

#endif //GLVIEW_POCKET_H
