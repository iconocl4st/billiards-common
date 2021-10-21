//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_REDUCE_GROBNER_H
#define IDEA_REDUCE_GROBNER_H

#include "algebra/poly_divide.h"

namespace algebra::poly {

	inline
	bool term_in_leading(int idx1, const Monomial& term, std::vector<PolyPtr>& polys) {
		for (int idx2 = 0; idx2 < polys.size(); idx2++) {
			if (idx1 == idx2) {
				continue;
			}
			const auto& ld2 = polys[idx2]->leading();
			if (ld2.first->divides(term.first)) {
				return true;
			}
		}
		return false;
	}

	inline
	void to_minimal(std::vector<PolyPtr>& polys) {
		bool reduced = true;
		while (reduced) {
			reduced = false;
			for (int idx1 = 0; idx1 < polys.size() && !reduced; idx1++) {
				const auto& term = polys[idx1]->leading();
				if (term_in_leading(idx1, term, polys)) {
					polys.erase(polys.begin() + idx1);
					reduced = true;
					break;
				}
			}
		}
	}

	inline
	void to_reduced(std::vector<PolyPtr>& polys) {
		bool reduced = true;
		while (reduced) {
			reduced = false;

			std::vector<PolyPtr> after_divisions;
			for (int i = 0; i < polys.size(); i++) {
				int current_size = (int) polys[i]->terms.size();
				Division d{polys[i]};
				for (int j = 0; j < polys.size(); j++) {
					if (i == j) {
						continue;
					}
					d.add_divisor(polys[j]);
				}
				divide(d);
				if (!d.check()) {
					throw std::runtime_error{"Could not divide..."};
				}
				auto rem = d.remainder;
				if (rem->terms.size() == current_size) {
					continue;
				}
				polys[i] = rem;
				reduced = true;
				break;
			}
		}
	}

	inline
	void reduce_grobner(std::vector<PolyPtr>& polys) {
		to_minimal(polys);
		to_reduced(polys);
	}
}


#endif //IDEA_REDUCE_GROBNER_H
