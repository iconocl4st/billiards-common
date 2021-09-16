//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_PARSING_PRIMITIVES_H
#define GLVIEW_PARSING_PRIMITIVES_H


namespace billiards::json {

	inline
	double parse_double(const nlohmann::json& value, const char *name) {
		assert(value.contains(name) && value[name].is_number());
		return value[name].get<double>();
	}

	inline
	double maybe_parse_double(const nlohmann::json& value, const char *name, double default_value) {
		if (!value.contains(name) || !value[name].is_number()) {
			return default_value;
		}
		return value[name].get<double>();
	}

	inline
	bool maybe_parse_boolean(const nlohmann::json& value, const char *name, bool default_value) {
		if (!value.contains(name) || !value[name].is_boolean()) {
			return default_value;
		}
		return value[name].get<bool>();
	}

	inline
	std::string parse_string(const nlohmann::json& value, const char *name) {
		assert(value.contains(name) && value[name].is_string());
		return value[name].get<std::string>();
	}

	inline
	const nlohmann::json& get_object(const nlohmann::json& value, const char *name) {
		assert(value.contains(name) && value[name].is_object());
		return value[name];
	}

	inline
	const nlohmann::json& get_array(const nlohmann::json& value, const char *name) {
		assert(value.contains(name) && value[name].is_array());
		return value[name];
	}
}



#endif //GLVIEW_PARSING_PRIMITIVES_H
