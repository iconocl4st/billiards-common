//
// Created by thallock on 9/14/21.
//

#ifndef GLVIEW_SAXREADER_H
#define GLVIEW_SAXREADER_H


namespace billiards::json {

	class SaxReader {
		SaxReader() = default;
		~SaxReader() = default;

		virtual void begin_object() = 0;
		virtual bool object_end() = 0;
		virtual void begin_array() = 0;
		virtual bool array_end() = 0;

		virtual std::string key() = 0;
		virtual bool is_null() = 0;
		virtual int int_value() = 0;
		virtual bool bool_value() = 0;

		virtual std::string string_value(const std::string& val) = 0;
		virtual double double_value(double val) = 0;
	};

}


#endif //GLVIEW_SAXREADER_H
