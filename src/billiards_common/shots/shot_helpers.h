//
// Created by thallock on 9/29/21.
//

#ifndef IDEA_SHOTHELPERS_H
#define IDEA_SHOTHELPERS_H

#include "billiards_common/config/Table.h"
#include "billiards_common/shots/Locations.h"

namespace billiards::shots {

	[[nodiscard]] inline
	int get_ball_type_index(
		const config::Table& table,
		const layout::Locations& locations,
		int index
	) {
		if (index < 0 || index >= locations.balls.size()) {
			throw std::runtime_error{"Invalid index"};
		}
		const auto& ball = locations.balls[index];
		switch (ball.ball.type) {
			case layout::vball::virtual_type::CUE:
				return 0; // TODO...
			case layout::vball::virtual_type::ANY_OBJECT:
				return 1; // TODO...
			case layout::vball::virtual_type::NUMBER:
				return ball.ball.number;
			case layout::vball::virtual_type::UNKNOWN:
			default:
				throw std::runtime_error{"Unknown virtual ball type"};
		}

	}

	[[nodiscard]] inline
	const config::BallInfo *get_ball_type(
		const config::Table& table,
		const layout::Locations& locations,
		int index
	) {
		int table_index = get_ball_type_index(table, locations, index);
		if (table_index < 0 || table_index >= table.balls.size()) {
			throw std::runtime_error{"Invalid index"};
		}
		return &table.balls[table_index];
	}
}

#endif //IDEA_SHOTHELPERS_H
