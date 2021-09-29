//
// Created by thallock on 9/27/21.
//

#ifndef IDEA_VIRTUALBALL_H
#define IDEA_VIRTUALBALL_H

#include "common/utils/Serializable.h"

namespace billiards::layout::vball {

	namespace virtual_type {
		enum VirtualBallType {
			CUE,
			ANY_OBJECT,
			NUMBER,
			UNKNOWN,
		};

		std::string to_string(VirtualBallType type) {
			switch (type) {
				case CUE:
					return "cue";
				case ANY_OBJECT:
					return "object";
				case NUMBER:
					return "number";
				case UNKNOWN:
				default:
					return "unknown";
			}
		}

		VirtualBallType from_string(const std::string& type) {
			if (type == "cue") {
				return CUE;
			} else if (type == "object") {
				return ANY_OBJECT;
			} else if (type == "number") {
				return NUMBER;
			} else {
				return UNKNOWN;
			}
		}
	}

	class VirtualBall : public json::Serializable {
	public:
		virtual_type::VirtualBallType type;
		int number;

		VirtualBall(int number) : type{virtual_type::NUMBER}, number{number} {}
		VirtualBall() : type{virtual_type::ANY_OBJECT}, number{-1} {}

		~VirtualBall() override = default;

		[[nodiscard]] bool is_cue() const {
			return type == virtual_type::CUE;
		}

		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("type", virtual_type::to_string(type));
			if (type == virtual_type::NUMBER) {
				writer.field("number", number);
			}
			writer.end_object();
		}

		inline
		void parse(const nlohmann::json& value) override {
			type = virtual_type::UNKNOWN;
			if (value.contains("type") && value["type"].is_string()) {
				type = virtual_type::from_string(value["type"].get<std::string>());
			}
			if (type == virtual_type::NUMBER && value.contains("number") && value["number"].is_number())
				number = value["number"].get<int>();
		}
	};
};

#endif //IDEA_VIRTUALBALL_H
