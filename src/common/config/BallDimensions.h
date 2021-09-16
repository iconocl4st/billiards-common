//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_BALLDIMENSIONS_H
#define GLVIEW_BALLDIMENSIONS_H

#include "../graphics/Color.h"

#include <array>

namespace billiards::config {

	enum BallType {
		SOLID,
		STRIPE,
		CUE
	};

	class BallInfo {
	public:
		BallType solid;
		double radius;
		gphx::Color color;
		int number;
		std::string name;

		BallInfo(BallType solid, double radius, gphx::Color color, int number, const char *name) :
			solid{solid},
			radius{radius},
			color{color},
			number{number},
			name{name} {}

		~BallInfo() = default;

	};

	namespace balls {
		inline
		std::array<BallInfo, 16> ALL_BALLS(){ return std::array<BallInfo, 16>{
			 BallInfo{   CUE, 2.26 / 2, gphx::color::from_int(255, 255, 255),  0, "cue"},
			 BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(255, 245,  64),  1, "one"},
			 BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int( 43,  59, 179),  2, "two"},
			 BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(255,   0,   0),  3, "three"},
			 BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int( 26,   1,  94),  4, "four"},
			 BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(255, 159,  41),  5, "five"},
			 BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(  9, 148,  30),  6, "six"},
			 BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(255,  25,  98),  7, "seven"},
			 BallInfo{ SOLID, 2.26 / 2, gphx::color::from_int(  0,   0,   0),  8, "eight"},
			 BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(255, 245,  64),  9, "nine"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int( 43,  59, 179), 10, "ten"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(255,   0,   0), 11, "eleven"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int( 26,   1,  94), 12, "twelve"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(255, 159,  41), 13, "thirteen"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(  9, 148,  30), 14, "fourteen"},
			BallInfo{STRIPE, 2.26 / 2, gphx::color::from_int(255,  25,  98), 15, "fifteen"},
		}; }
	}
}

#endif //GLVIEW_BALLDIMENSIONS_H
