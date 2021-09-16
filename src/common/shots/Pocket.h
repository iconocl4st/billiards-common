//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_POCKET_H
#define GLVIEW_POCKET_H

#include "../geometry/Point.h"

namespace billiards::shots {

	class Pocket {
	public:
		Point outerSegment1;
		Point outerSegment2;
		Point innerSegment1;
		Point holeCenter;

		inline
		Point innerSegment2() {
			Line segment = geometry::through(outerSegment1, outerSegment2);
			Line normal = geometry::orthogonal_at(segment, (outerSegment1 + outerSegment2) / 2);
			return geometry::reflect(innerSegment1, normal);
		}

	};
}

#endif //GLVIEW_POCKET_H
