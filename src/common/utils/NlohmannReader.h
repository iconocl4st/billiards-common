//
// Created by thallock on 9/14/21.
//

#ifndef GLVIEW_NLOHMANNREADER_H
#define GLVIEW_NLOHMANNREADER_H

#include "./SaxReader.h"
#include "../3rd_party/nlohmann.hpp"

namespace billiards::json {

	class NlohmannReader : public SaxReader {
	private:
		nlohmann::json root;

	public:
		NlohmannReader(std::string& string) root{string} {};

		void begin_object() override;
		bool object_end() override;
		void begin_array() override;
		bool array_end() override;

		std::string key() override;
		bool is_null() override;
		int int_value() override;
		bool bool_value() override;
		std::string string_value() override;
		double double_value() override;
	};
}


#endif //GLVIEW_NLOHMANNREADER_H
