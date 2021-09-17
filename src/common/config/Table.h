//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_TABLE_H
#define IDEA_TABLE_H

#include "./Ball.h"
#include "./Pocket.h"
#include "./Rail.h"

namespace billiards::config {
	class Table : public json::Serializable {
	public:
		std::array<BallInfo, 16> balls;
		std::array<Pocket, 6> pockets;
		std::array<Rail, 6> rails;

		Table() = default;
		virtual ~Table() = default;

		void to_json(json::SaxWriter& writer) const {

		};

		void parse(const nlohmann::json& value) {

		};
	};
}
#endif //IDEA_TABLE_H
