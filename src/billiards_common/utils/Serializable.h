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

        ParseResult(bool success) : success{success}, error_msg{} {}
        ParseResult() : ParseResult(true) {}
//		ParseResult(const char *msg) : success{false}, error_msg{} {
//			error_msg << msg;
//		}
//		ParseResult(const std::string& child, ParseResult cause) {
//			success = false;
//			error_msg << child << ": ";
//			error_msg << cause.error_msg.str();
//		}

        ~ParseResult() = default;
    };

#define HAS_NUMBER(value, name) (value.contains(name) && value[name].is_number())
#define HAS_BOOL(value, name) (value.contains(name) && value[name].is_boolean())
#define HAS_OBJECT(value, name) (value.contains(name) && value[name].is_object())
#define HAS_STRING(value, name) (value.contains(name) && value[name].is_string())
#define HAS_ARRAY(value, name) (value.contains(name) && value[name].is_array())

#define ENSURE_NUMBER(result, value, name, msg)  do { 			\
	if (!HAS_NUMBER(value, name)) {								\
        result.success = false;                        			\
		result.error_msg << msg;								\
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
#define ENSURE_OBJECT(result, value, name, msg)  do { 			\
	if (!HAS_OBJECT(value, name)) {								\
        result.success = false;                        			\
		result.error_msg << msg;								\
		return;			 										\
	}               											\
} while (false)
#define ENSURE_STRING(result, value, name, msg)  do { 			\
	if (!HAS_STRING(value, name)) {								\
        result.success = false;                        			\
		result.error_msg << msg;								\
		return;			 										\
	}               											\
} while (false)
#define ENSURE_ARRAY(result, value, name, msg)  do { 			\
	if (!HAS_ARRAY(value, name)) {								\
        result.success = false;                        			\
		result.error_msg << msg;								\
		return;			 										\
	}               											\
} while (false)
#define PARSE_CHILD(result, value, member)  do {				\
	member.parse(value, result);								\
	if (!result.success) {                                  	\
    	return;                                                 \
	}                                                         	\
} while (false)

#define REQUIRE_CHILD(result, value, key, member, msg)  do {	\
    ENSURE_OBJECT(result, value, key, msg);						\
    PARSE_CHILD(result, value[key], member);					\
} while (false)

    class Serializable {
    public:
        Serializable() = default;
        virtual ~Serializable() = default;

        virtual void to_json(json::SaxWriter& writer) const = 0;

        virtual void parse(const nlohmann::json& value, ParseResult& result) = 0;

        // TODO: change this to a SaxReader instead of a nlohmann::json
    };
}


#endif //GLVIEW_SERIALIZABLE_H
