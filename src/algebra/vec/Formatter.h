//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_FORMATTER_H
#define IDEA_FORMATTER_H

#include <functional>
#include <sstream>

namespace algebra::poly {
	typedef std::function<std::string(int index)> Formatter;

	[[nodiscard]] inline
	Formatter default_formatter() {
		return [](int index) {
			std::stringstream ss;
//			ss << index << ",";
			ss << "x[" << index << "]";
			return ss.str();
		};
	}
}

#endif //IDEA_FORMATTER_H
