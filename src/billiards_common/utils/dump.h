//
// Created by thallock on 9/14/21.
//

#ifndef GLVIEW_DUMP_H
#define GLVIEW_DUMP_H


#include "billiards_common/utils/Serializable.h"
#include "billiards_common/utils/NlohmannWriter.h"

namespace billiards::json {

	std::string dump(const Serializable& obj) {
		NlohmannWriter writer;
		SaxWriter& w{writer};
		obj.to_json(w);
		return writer.get_root().dump();
	}

	std::string pretty_dump(const Serializable& obj) {
		NlohmannWriter writer;
		SaxWriter& w{writer};
		obj.to_json(w);
		return writer.get_root().dump(2);
	}

//	std::string parse(const Serializable& obj) {
//		NlohmannReader reader;
//		SaxWriter& w{writer};
//		obj.to_json(w);
//		return writer.get_root().dump();
//	}
}


#endif //GLVIEW_DUMP_H
