//
// Created by thallock on 11/11/21.
//

#ifndef IDEA_CURRENT_TIME_H
#define IDEA_CURRENT_TIME_H

#include <chrono>
#include <cinttypes>

namespace billiards::utils {
	[[nodiscard]] inline
	uint64_t now() {
		const auto r1 = std::chrono::system_clock::now();
//		const auto r2 = std::chrono::system_clock::to_time_t(r1);
		const auto r2 = r1.time_since_epoch();
		const auto r3 = std::chrono::duration_cast<std::chrono::seconds>(r2);
		const auto r4 = r3.count();
		return r4;
	}
}
#endif //IDEA_CURRENT_TIME_H
