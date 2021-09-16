//
// Created by thallock on 9/14/21.
//

#ifndef GLVIEW_IO_H
#define GLVIEW_IO_H


#include <sstream>
#include <fstream>

namespace billiards::util {

	std::string read_file(const char *filename) {
		std::ostringstream buff;

		std::string line;
		std::ifstream input_file{filename};
		if (!input_file.is_open()) {
			return "";
		}

		while (getline(input_file, line)) {
			buff << line << '\n';
		}

		return buff.str();
	}
}

#endif //GLVIEW_IO_H
