//
// Created by thallock on 9/7/21.
//

#ifndef GLVIEW_SAXWRITER_H
#define GLVIEW_SAXWRITER_H

#include <string>
#include <cctype>

namespace billiards::json {

	class SaxWriter {
	public:

		SaxWriter() = default;
		virtual ~SaxWriter() = default;

		virtual void begin_object() = 0;
		virtual void end_object() = 0;
		virtual void begin_array() = 0;
		virtual void end_array() = 0;

//		virtual void begin_object(const std::string& key) = 0;
//		virtual void begin_array(const std::string& key) = 0;

		virtual void key(const std::string &str) = 0;

		virtual void value(int val) = 0;
		virtual void value(uint64_t val) = 0;
		virtual void value(uint8_t val) = 0;
		virtual void value(bool val) = 0;
		virtual void value(const std::string& val) = 0;
		virtual void value(double val) = 0;
		virtual void null() = 0;

		virtual void string_value(const std::string& val) {
			value(val);
		}

		virtual void field(const std::string &key, const std::string& val) = 0;
		virtual void field(const std::string &key, int val) = 0;
		virtual void field(const std::string &key, bool val) = 0;
		virtual void field(const std::string &key, double val) = 0;
		virtual void field(const std::string &key, uint64_t val) = 0;
		virtual void field(const std::string &key, uint8_t val) = 0;

		virtual void null_field(const std::string &key) = 0;

		virtual void string_field(const std::string& key, const std::string& val) {
			field(key, val);
		}

		virtual void value(char *val) { string_value(val);}
		virtual void field(const std::string& key, char *val) { string_field(key, val);}
		virtual void value(const char *val) { string_value(val);}
		virtual void field(const std::string& key, const char *val) { string_field(key, val);}

	};

}


#endif //GLVIEW_SAXWRITER_H
