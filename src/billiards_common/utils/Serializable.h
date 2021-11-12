//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_SERIALIZABLE_H
#define GLVIEW_SERIALIZABLE_H

#include <sstream>

#include "billiards_common/3rd_party/nlohmann.hpp"
#include "billiards_common/utils/SaxWriter.h"

namespace billiards::json {

    class ParseResult {
    public:
        bool success;
        std::stringstream error_msg;

        explicit ParseResult(bool success) : success{success}, error_msg{} {}
        ParseResult() : ParseResult(true) {}
        virtual ~ParseResult() = default;
    };

    class Serializable {
    public:
        Serializable() = default;
        virtual ~Serializable() = default;

        virtual void to_json(json::SaxWriter& writer) const = 0;

        virtual void parse(const nlohmann::json& value, ParseResult& result) = 0;

        // TODO: change this to a SaxReader instead of a nlohmann::json
    };



#define HAS_NUMBER(value, name) (value.contains(name) && value[name].is_number())
#define HAS_BOOL(value, name) (value.contains(name) && value[name].is_boolean())
#define HAS_OBJECT(value, name) (value.contains(name) && value[name].is_object())
#define HAS_STRING(value, name) (value.contains(name) && value[name].is_string())
#define HAS_ARRAY(value, name) (value.contains(name) && value[name].is_array())

#define ENSURE_NUMBER(status, value, name, msg)  do { 			\
	if (!HAS_NUMBER(value, name)) {								\
        status.success = false;                        			\
		status.error_msg << msg;								\
		return;					 								\
	}               											\
} while (false)
#define ENSURE_BOOL(result, value, name, msg)  do { 			\
	if (!HAS_BOOL(value, name)) {                         		\
        result.success = false;                        			\
		result.error_msg << msg;								\
		return;					 								\
	}               											\
} while (false)
#define ENSURE_OBJECT(status, value, name, msg)  do { 			\
	if (!HAS_OBJECT(value, name)) {								\
        status.success = false;                        			\
		status.error_msg << msg;								\
		return;			 										\
	}               											\
} while (false)
#define ENSURE_STRING(status, value, name, msg)  do { 			\
	if (!HAS_STRING(value, name)) {								\
        status.success = false;                        			\
		status.error_msg << msg;								\
		return;			 										\
	}               											\
} while (false)
#define ENSURE_ARRAY(status, value, name, msg)  do { 			\
	if (!HAS_ARRAY(value, name)) {								\
        status.success = false;                        			\
		status.error_msg << msg;								\
		return;			 										\
	}               											\
} while (false)
#define PARSE_CHILD(status, value, member)  do {				\
	member.parse(value, status);								\
	if (!status.success) {                                  	\
    	return;                                                 \
	}                                                         	\
} while (false)

#define REQUIRE_CHILD(result, value, key, member, msg)  do {	\
    ENSURE_OBJECT(result, value, key, msg);						\
    PARSE_CHILD(result, value[key], member);					\
} while (false)

}


#endif //GLVIEW_SERIALIZABLE_H
