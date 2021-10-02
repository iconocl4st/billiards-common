//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_LINE_H
#define GLVIEW_LINE_H

#include <cmath>
#include <optional>

#include "billiards_common/geometry/tolerance.h"

namespace billiards::geometry {

	class Line {
	public:
		// Represents the set of points that satisfy a * x + b * y + c == 0
		double a, b, c;

		Line(double a, double b, double c) : a{a}, b{b}, c{c} {}

		~Line() = default;

		[[nodiscard]] inline
		std::optional<Line> normalize() const {
			double n = std::sqrt(a * a + b * b);
			if (n < TOLERANCE) {
				return {};
			}
			if (c >= 0.0) {
				return Line{a / n, b / n, c / n};
			} else {
				return Line{-a / n, -b / n, -c / n};
			}
		}
	};

}



#endif //GLVIEW_LINE_H
