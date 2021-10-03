//
// Created by thallock on 9/27/21.
//

#ifndef IDEA_VIRTUALBALL_H
#define IDEA_VIRTUALBALL_H

#include "billiards_common/utils/Serializable.h"

namespace billiards::layout::vball {

	namespace virtual_type {
		enum VirtualBallType {
			CUE,
			ANY_OBJECT,
			NUMBER,
			UNKNOWN,
		};

		[[nodiscard]] inline
		bool is_cue_ball(VirtualBallType type) {
			switch (type) {
				case CUE:
					return true;
				case NUMBER:
				case ANY_OBJECT:
					return false;
				case UNKNOWN:
				default:
					throw std::runtime_error{"Unknown virtual ball type"};
			}
		}

		[[nodiscard]] inline
		bool is_object_ball(VirtualBallType type) {
			switch (type) {
				case NUMBER:
				case ANY_OBJECT:
					return true;
				case CUE:
					return false;
				case UNKNOWN:
				default:
					throw std::runtime_error{"Unknown virtual ball type"};
			}
		}

		[[nodiscard]] inline
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

		[[nodiscard]] inline
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

		VirtualBall(virtual_type::VirtualBallType type, int number)
			: type{type}
			, number{number}
		{}
		VirtualBall(int number) : VirtualBall{virtual_type::NUMBER, number} {}
		VirtualBall() : VirtualBall{virtual_type::ANY_OBJECT, -1} {}

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
		void parse(const nlohmann::json& value, json::ParseResult& result) override {
			ENSURE_STRING(result, value, "type", "Virtual ball must have a type");
			type = virtual_type::from_string(value["type"].get<std::string>());
			if (type == virtual_type::NUMBER && HAS_NUMBER(value, "number")) {
				number = value["number"].get<int>();
			}
		}
	};
};

#endif //IDEA_VIRTUALBALL_H
