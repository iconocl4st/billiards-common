//
// Created by thallock on 9/7/21.
//

#ifndef GLVIEW_SAXWRITER_H
#define GLVIEW_SAXWRITER_H

#include <string>

namespace billiards::json {

	class SaxWriter {

	public:
		SaxWriter() = default;
		~SaxWriter() = default;

		virtual void begin_object() = 0;
		virtual void end_object() = 0;
		virtual void begin_array() = 0;
		virtual void end_array() = 0;

//		virtual void begin_object(const std::string& key) = 0;
//		virtual void begin_array(const std::string& key) = 0;

		virtual void key(const std::string &str) = 0;

		virtual void value(int val) = 0;
		virtual void value(bool val) = 0;
		virtual void value(const std::string &val) = 0;
		virtual void value(double val) = 0;
		virtual void null() = 0;

		virtual void field(const std::string &key, const std::string& val) = 0;
		virtual void field(const std::string &key, int val) = 0;
		virtual void field(const std::string &key, bool val) = 0;
		virtual void field(const std::string &key, double val) = 0;
		virtual void null_field(const std::string &key) = 0;


		virtual void value(char *val) { value(std::string{val});}
		virtual void field(const std::string& key, char *val) { field(key, std::string{val});}
		virtual void value(const char *val) { value(std::string{val});}
		virtual void field(const std::string& key, const char *val) { field(key, std::string{val});}
	};

}


#endif //GLVIEW_SAXWRITER_H
