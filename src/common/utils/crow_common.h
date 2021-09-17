//
// Created by thallock on 9/16/21.
//

#ifndef GLVIEW_DEFAULT_RESONSE_H
#define GLVIEW_DEFAULT_RESONSE_H

#include "./dump.h"

namespace billiards::utils {

	
	class DefaultResponse : public json::Serializable {
	public:
		std::string message;
		bool success;
		std::string key;
		const json::Serializable *data;
		
		
		DefaultResponse(const std::string& message, bool success, const std::string& key, const json::Serializable *data)
			: message{message}
			, success{success}
			, key{key}
			, data{data}
			{}
			
		DefaultResponse(const std::string& message, bool success, json::Serializable *data)
			: DefaultResponse{message, success, "data", data} {}
		
		DefaultResponse(const std::string& message, bool success)
			: DefaultResponse{message, success, nullptr} {}
			
		DefaultResponse(const std::string& message)
			: DefaultResponse{message, true, nullptr} {}

		
// 		std::string dump() override {
// 			crow::response resp{billiards::json::dump(locals::front_end->display.location)};
// 			resp.add_header("Access-Control-Allow-Origin", "*");
// 			return resp;
// 		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("message", message);
			writer.field("success", success);
			if (data != nullptr) {
				writer.key(key);
				data->to_json(writer);
			}
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value) override {
			// This should not need to be called...
		}
	};
}


#define HANDLE_OPTIONS do {													\
	crow::response resp;													\
	resp.add_header("Access-Control-Allow-Origin", "*");					\
	resp.add_header("Access-Control-Allow-Headers", "Content-Type");		\
	resp.add_header("Access-Control-Allow-Methods", "PUT, GET, OPTIONS");	\
	return resp;															\
} while (false)


#define RETURN_SUCCESS_WITH_DATA(msg, data) do {			\
	billiards::utils::DefaultResponse dr{msg, true, &data};	\
	crow::response resp{billiards::json::dump(dr)};			\
	resp.add_header("Access-Control-Allow-Origin", "*");	\
	return resp;			 								\
} while (false)


#define RETURN_SUCCESS(msg) do {							\
	crow::json::wvalue x; 									\
	x["success"] = true;									\
	x["message"] = msg;             						\
    crow::response resp{x};									\
	resp.add_header("Access-Control-Allow-Origin", "*");	\
	return resp;			 								\
} while (false)

#define RETURN_FAILURE(msg) do {							\
	crow::json::wvalue x; 									\
	x["success"] = false;									\
	x["message"] = msg;             						\
    crow::response resp{x};									\
	resp.add_header("Access-Control-Allow-Origin", "*");	\
	return resp;			 								\
} while (false)


#endif // GLVIEW_DEFAULT_RESONSE_H
