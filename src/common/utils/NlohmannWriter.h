//
// Created by thallock on 9/7/21.
//

#ifndef GLVIEW_NLOHMANN_WRITER_H
#define GLVIEW_NLOHMANN_WRITER_H

#include "SaxWriter.h"

#include <list>
#include <iostream>

#include "../3rd_party/nlohmann.hpp"

namespace billiards::json {

	enum ContextType {
		IN_OBJECT,
		IN_ARRAY,
	};

	class NlohmannWriter : public SaxWriter {
	private:
		std::list<std::pair<ContextType, nlohmann::json&>> stack;
		std::string current_key;
		nlohmann::json root;

	public:
		NlohmannWriter() : stack{}, current_key{} {};
		~NlohmannWriter() = default;

		void debug_stack() const {
			for (const auto & it : stack) {
				std::cout << (it.first == IN_OBJECT ? "o" : "a") << "," << it.second << '\n';
			}
			std::cout << "=============================================" << std::endl;
		}

		void begin_object() override {
			if (stack.empty()) {
				root = nlohmann::json::object();
				stack.emplace_back(IN_OBJECT, root);
			} else if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = nlohmann::json::object();
				stack.emplace_back(IN_OBJECT, stack.back().second[current_key]);
			} else if (stack.back().first == IN_ARRAY) {
				auto len = stack.back().second.size();
				stack.back().second.push_back(nlohmann::json::object());
				stack.emplace_back(IN_OBJECT, stack.back().second.back());
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}

		void end_object() override {
			stack.pop_back();
		}

		void begin_array() override {
			if (stack.empty()) {
				root = nlohmann::json::array();
				stack.emplace_back(IN_ARRAY, root);
			} else if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = nlohmann::json::array();
				stack.emplace_back(IN_ARRAY, stack.back().second[current_key]);
			} else if (stack.back().first == IN_ARRAY) {
				auto len = stack.back().second.size();
				stack.back().second.push_back(nlohmann::json::array());
				stack.emplace_back(IN_ARRAY, stack.back().second[len]);
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}
		void end_array() override {
			end_object();
		}

		void key(const std::string &str) override {
			current_key = str;
		}

		void value(int val) override {
			if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = val;
			} else if (stack.back().first == IN_ARRAY) {
				stack.back().second.push_back(val);
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}

		void value(uint8_t val) override {
			if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = val;
			} else if (stack.back().first == IN_ARRAY) {
				stack.back().second.push_back(val);
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}

		void value(uint64_t val) override {
			if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = val;
			} else if (stack.back().first == IN_ARRAY) {
				stack.back().second.push_back(val);
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}

		void value(bool val) override {
			if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = val;
			} else if (stack.back().first == IN_ARRAY) {
				stack.back().second.push_back(val);
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}
		void value(const std::string &val) override {
			if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = val;
			} else if (stack.back().first == IN_ARRAY) {
				stack.back().second.push_back(val);
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}
		void value(double val) override {
			if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = val;
			} else if (stack.back().first == IN_ARRAY) {
				stack.back().second.push_back(val);
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}
		void null() override {
			if (stack.back().first == IN_OBJECT) {
				stack.back().second[current_key] = nullptr;
			} else if (stack.back().first == IN_ARRAY) {
				stack.back().second.push_back(nullptr);
			} else {
				std::cerr << "Unknown context" << std::endl;
			}
		}

		void field(const std::string &key, const std::string &val) override {
			stack.back().second[key] = val;
		}
		void field(const std::string &key, int val) override {
			stack.back().second[key] = val;
		}
		void field(const std::string &key, uint8_t val) override {
			stack.back().second[key] = val;
		}
		void field(const std::string &key, uint64_t val) override {
			stack.back().second[key] = val;
		}
		void field(const std::string &key, bool val) override {
			stack.back().second[key] = val;
		}
		void field(const std::string &key, double val) override {
			stack.back().second[key] = val;
		}
		void null_field(const std::string &key) override {
			stack.back().second[key] = nullptr;
		}

		void value(const char *val) override { value(std::string{val});}
		void field(const std::string& key, const char *val) override { field(key, std::string{val});}

		[[nodiscard]] inline
		const nlohmann::json& get_root() const { return root; }
	};
}

#endif //GLVIEW_NLOHMANN_WRITER_H
