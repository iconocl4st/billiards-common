//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_PAIR_H
#define IDEA_PAIR_H

namespace algebra::poly {
	class Pair {
	public:
		int idx1;
		int idx2;

		Pair(int idx1, int idx2) : idx1{idx1}, idx2{idx2} {}

		~Pair() = default;

		[[nodiscard]] inline
		bool operator<(const Pair& other) const {
			if (idx1 < other.idx1) {
				return true;
			}
			if (idx1 > other.idx1) {
				return false;
			}
			if (idx2 < other.idx2) {
				return true;
			}
			return false;
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const Pair& p) {
			return os << "{i:" << p.idx1 << ",j:" << p.idx2 << "}";
		}
	};
}
#endif //IDEA_PAIR_H
