//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_CHECK_GROBNER_H
#define IDEA_CHECK_GROBNER_H

#include "algebra/poly_divide.h"

namespace algebra::poly {

	[[nodiscard]] inline
	PolyPtr construct_s(const PolyPtr& p1, const PolyPtr& p2) {
		auto ld1 = p1->leading();
		auto ld2 = p2->leading();
		IndexPtr lcm = ld1.first->lcm(ld2.first);
		PolyPtr s = (p1 * Monomial(lcm->div(ld1.first), ld2.second)) - (p2 * Monomial(lcm->div(ld2.first), ld1.second));
//		std::cout << "lcm(" << ld1 << ", " << ld2 << ") = " << *lcm << std::endl;
//		std::cout << "Computed S(" << p1 << ", " << p2 << ") = " << s << std::endl;
		return s;
	}

	[[nodiscard]] inline
	bool is_grobner(std::vector<PolyPtr>& basis) {
		for (int i = 0; i < basis.size(); i++) {
			for (int j = 0; j < basis.size(); j++) {
				if (i == j) {
					continue;
				}

				PolyPtr p1 = basis[i];
				PolyPtr p2 = basis[j];
				PolyPtr s = construct_s(p1, p2);

				Division d{s};
				for (const auto& b : basis) {
					d.add_divisor(b);
				}
				divide(d);
				PolyPtr rem = d.remainder;
				if (!d.check()) {
					throw std::runtime_error{"invalid division"};
				}
				if (rem->is_zero()) {
					continue;
				}

				std::cout << "For pair: " << i << ", " << j << ":" << std::endl;
				std::cout << "\tS = " << s << "\n\tR = " << rem << std::endl;
				return false;
			}
		}
		return true;
	}
}

#endif //IDEA_CHECK_GROBNER_H
