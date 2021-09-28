//
// Created by thallock on 9/27/21.
//

#ifndef IDEA_VIRTUALBALL_H
#define IDEA_VIRTUALBALL_H

#include "common/utils/Serializable.h"

namespace billiards::layout::vball {

	enum VirtualBallType {
		CUE,
		ANY_OBJECT,
		NUMBER,
		UNKNOWN,
	};

	std::string virtual_ball_to_string(VirtualBallType type) {
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

	VirtualBallType string_to_virtual_ball(const std::string& type) {
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

	class VirtualBall : public json::Serializable {
	public:
		VirtualBallType type;
		int number;

		VirtualBall(int number) : type{vball::NUMBER}, number{number} {}
		VirtualBall() : type{vball::ANY_OBJECT}, number{-1} {}

		~VirtualBall() override = default;

		[[nodiscard]] bool is_cue() const {
			return type == CUE;
		}

		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("type", vball::virtual_ball_to_string(type));
			if (type == vball::NUMBER) {
				writer.field("number", number);
			}
			writer.end_object();
		}

		inline
		void parse(const nlohmann::json& value) override {
			type = vball::UNKNOWN;
			if (value.contains("type") && value["type"].is_string()) {
				type = vball::string_to_virtual_ball(value["type"].get<std::string>());
			}
			if (type == vball::NUMBER && value.contains("number") && value["number"].is_number())
				number = value["number"].get<int>();
		}
	};
};

#endif //IDEA_VIRTUALBALL_H
