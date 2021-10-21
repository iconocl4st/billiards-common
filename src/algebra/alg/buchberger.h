//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_GROEBNER_H
#define IDEA_GROEBNER_H

#include <list>
#include <fstream>

#include "algebra/poly_divide.h"
#include "algebra/alg/Pair.h"
#include "algebra/alg/reduce_grobner.h"

namespace algebra::poly {

	[[nodiscard]] inline
	PolyPtr construct_s(const PolyPtr& p1, const PolyPtr& p2) {
		auto ld1 = p1->leading();
		auto ld2 = p2->leading();
		IndexPtr lcm = ld1.first->lcm(ld2.first);
		PolyPtr s = (p1 * Monomial(lcm->div(ld1.first), ld2.second)) - (p2 * Monomial(lcm->div(ld2.first), ld1.second));
		std::cout << "lcm(" << ld1 << ", " << ld2 << ") = " << *lcm << std::endl;
		std::cout << "Computed S(" << p1 << ", " << p2 << ") = " << s << std::endl;
		return s;
	}


	inline
	void add_all_less(std::list<Pair>& queue, int index) {
		for (int i = 0; i < index; i++) {
			queue.emplace_back(i, index);
		}
	}

//	[[nodiscard]] inline
//	bool leading_term_in(PolyVec rem, std::vector<PolyVec>& polynomials) {
//		for (PolyVec& g : polynomials) {
//			if (rem.terms[0].is_divisible_by(g.terms[0])) {
//				return true;
//			}
//		}
//		return false;
//	}

	inline
	void reduce(std::vector<PolyPtr>& polynomials) {
	}

	inline
	void buchberger(std::vector<PolyPtr>& polynomials) {
		std::list<Pair> queue;
		for (int i = 0; i < polynomials.size(); i++) {
			add_all_less(queue, i);
		}

		int count = 0;
		while (!queue.empty()) {
			Pair p = queue.front();
			queue.pop_front();

			PolyPtr s = construct_s(polynomials[p.idx1], polynomials[p.idx2]);
			if (s->is_zero()) {
				continue;
			}
			Division d{s};
			for (const auto& poly : polynomials) {
				d.add_divisor(poly);
			}
			divide(d);
			PolyPtr rem = d.remainder;
			std::cout << "Remainder is " << rem << std::endl;

			if (!d.check()) {
				throw std::runtime_error{"invalid division"};
			}
			if (rem->is_zero()) {
				std::cout << "\t...is zero" << std::endl;
				continue;
			}

			std::cout << "Found num " << count++ << ":" << std::endl;
			std::cout << "\t" << rem << std::endl;
			int index = (int) polynomials.size();
			add_all_less(queue, index);
			polynomials.push_back(rem->to_monic());
		}

		reduce_grobner(polynomials);
	}
}

#endif //IDEA_GROEBNER_H
