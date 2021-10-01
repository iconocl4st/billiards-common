//
// Created by thallock on 9/16/21.
//

#ifndef GLVIEW_DEFAULT_RESONSE_H
#define GLVIEW_DEFAULT_RESONSE_H

#include "dump.h"

namespace billiards::utils {

	
	class DefaultResponse : public json::Serializable {
	public:
		std::string message;
		bool success;
		std::string key;
		const json::Serializable *data;
		std::function<void(json::SaxWriter&)> callable;
		bool use_callable;


		DefaultResponse(const std::string& message, bool success, const std::string& key, std::function<void(json::SaxWriter&)> callable)
			: message{message}
			, success{success}
			, key{key}
			, data{nullptr}
			, callable{callable}
			, use_callable{true}
		{}

		DefaultResponse(const std::string& message, bool success, const std::string& key, const json::Serializable *data)
			: message{message}
			, success{success}
			, key{key}
			, data{data}
			, callable{[](json::SaxWriter&){}}
			, use_callable{false}
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
			if (use_callable) {
				writer.key(key);
				callable(writer);
			} else if (data != nullptr) {
				writer.key(key);
				data->to_json(writer);
			}
			writer.end_object();
		}
		
		void parse(const nlohmann::json& value, json::ParseResult& result) override {
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


#define RETURN_SUCCESS_WITH_DATA(msg, name, data) do {				\
	billiards::utils::DefaultResponse dr{msg, true, name, &data};	\
	crow::response resp{billiards::json::dump(dr)};          		\
	resp.set_header("Content-Type", "application/json");			\
	resp.set_header("Mime-type", "application/json");               \
	resp.add_header("Access-Control-Allow-Origin", "*");			\
	return resp;			 										\
} while (false)

#define RETURN_ERROR(msg) do {								\
	billiards::utils::DefaultResponse dr{msg, false};		\
	crow::response resp{billiards::json::dump(dr)};			\
	resp.add_header("Access-Control-Allow-Origin", "*");	\
	resp.set_header("Content-Type", "application/json");	\
	resp.set_header("Mime-type", "application/json");       \
	return resp;			 								\
} while (false)


#define RETURN_SUCCESS(msg) do {							\
	crow::json::wvalue x; 									\
	x["success"] = true;									\
	x["message"] = msg;             						\
    crow::response resp{x};									\
	resp.set_header("Content-Type", "application/json");	\
	resp.set_header("Mime-type", "application/json");       \
	resp.add_header("Access-Control-Allow-Origin", "*");	\
	return resp;			 								\
} while (false)

#define RETURN_FAILURE(msg) do {							\
	crow::json::wvalue x; 									\
	x["success"] = false;									\
	x["message"] = msg;             						\
    crow::response resp{x};									\
	resp.set_header("Content-Type", "application/json");	\
	resp.set_header("Mime-type", "application/json");       \
	resp.add_header("Access-Control-Allow-Origin", "*");	\
	return resp;			 								\
} while (false)

// TODO:
// Shutdown stops the processes from returning...
#define DO_STATUS_ENDPOINT() do {												\
	std::mutex shutdown_mutex;													\
	const uint64_t start_time = std::time(0);									\
	CROW_ROUTE(app, "/status/")													\
		.methods("GET"_method, "PUT"_method, "OPTIONS"_method)					\
		([start_time, &app, &shutdown_mutex](const crow::request& req) {		\
		if (req.method == "OPTIONS"_method) {									\
			HANDLE_OPTIONS;														\
		} else if (req.method == "GET"_method) {								\
			crow::json::wvalue x;												\
			x["success"] = true;												\
			x["message"] = "retrieved uptime";									\
			x["up-time"] = (std::time(0) - start_time);							\
			crow::response resp{x};												\
			resp.set_header("Content-Type", "application/json");				\
			resp.set_header("Mime-type", "application/json");       			\
			resp.add_header("Access-Control-Allow-Origin", "*");				\
			return resp;														\
		} else if (req.method == "PUT"_method) {								\
			nlohmann::json value = nlohmann::json::parse(req.body);				\
			if (value.contains("shutdown")										\
				&& value["shutdown"].is_boolean()								\
				&& value["shutdown"].get<bool>()								\
			) {																	\
				app.stop();														\
				RETURN_SUCCESS("Shutting down");								\
			}																	\
			RETURN_SUCCESS("Nothing to do");									\
		} else {																\
			return crow::response(404);											\
		}																		\
	});                                  										\
} while (false)



#endif // GLVIEW_DEFAULT_RESONSE_H
