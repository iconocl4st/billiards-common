//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOT_H
#define GLVIEW_SHOT_H

namespace billiards::shots {

	enum SegmentType {
		Ghost,
//		Kick,
//		Bank,
		Rail,
		Carem,
		Pocket,
	};

	class PocketSegment {

	};
	class RailSegment {

	};
	class GhostSegment {

	};

	class ShotSegment {
		bool transfer;
		SegmentType type;
	};

	class Shot {


	};

}

#endif //GLVIEW_SHOT_H
