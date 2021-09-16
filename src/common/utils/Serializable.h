//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_SERIALIZABLE_H
#define GLVIEW_SERIALIZABLE_H

#include "SaxWriter.h"

#include "../3rd_party/nlohmann.hpp"
#include <sstream>

namespace billiards::json {

	class Serializable {
	public:
		Serializable() = default;;
		virtual ~Serializable() = default;

		virtual void to_json(SaxWriter& writer) const = 0;
// TODO: change this to a SaxReader
		virtual void parse(const nlohmann::json& value) = 0;
	};
}


#endif //GLVIEW_SERIALIZABLE_H
