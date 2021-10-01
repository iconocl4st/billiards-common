//
// Created by thallock on 9/14/21.
//

#ifndef GLVIEW_DUMP_H
#define GLVIEW_DUMP_H


#include "Serializable.h"
#include "NlohmannWriter.h"

namespace billiards::json {

	std::string dump(const Serializable& obj) {
		NlohmannWriter writer;
		SaxWriter& w{writer};
		obj.to_json(w);
		return writer.get_root().dump();
	}

//	std::string parse(const Serializable& obj) {
//		NlohmannReader reader;
//		SaxWriter& w{writer};
//		obj.to_json(w);
//		return writer.get_root().dump();
//	}
}


#endif //GLVIEW_DUMP_H
