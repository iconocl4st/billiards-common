//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_GROEBNER_H
#define IDEA_GROEBNER_H

#include "Polynomial.h"
#include "poly_divide.h"
#include <list>
#include <fstream>

namespace algebra::poly {
	class Pair {
	public:
		int idx1;
		int idx2;

		Pair(int idx1, int idx2) : idx1{idx1}, idx2{idx2} {}
		~Pair() = default;
	};

	[[nodiscard]] inline
	Polynomial construct_s(const Polynomial& p1, const Polynomial& p2) {
		Monomial lcm = p1.terms[0].lcm(p2.terms[0]);
//		std::cout << "lcm(" << p1.terms[0] << ", " << p2.terms[0] << ") = " << lcm << std::endl;
		return (p1 * (lcm / p1.terms[0]) + p2 * (lcm / p2.terms[0])).canon();
	}

	[[nodiscard]] inline
	bool leading_term_in(Polynomial rem, std::vector<Polynomial>& polynomials) {
		for (Polynomial& g : polynomials) {
			if (rem.terms[0].is_divisible_by(g.terms[0])) {
				return true;
			}
		}
		return false;
	}

	inline
	void add_all_less(std::list<Pair>& queue, int index) {
		for (int i = 0; i < index; i++) {
			queue.emplace_back(i, index);
		}
	}

	inline
	void buchberger(std::vector<Polynomial>& polynomials) {
		std::list<Pair> queue;
		for (int i = 0; i < polynomials.size(); i++) {
			add_all_less(queue, i);
		}
		std::ofstream outf{"basis_polys.txt"};
		int count = 0;
		while (!queue.empty()) {
			Pair p = queue.front();
			queue.pop_front();

			Polynomial s = construct_s(polynomials[p.idx1], polynomials[p.idx2]);
			Division d{polynomials, s};
			divide(d);
			Polynomial rem = d.remainder.canon();
			if (!d.check()) {
//				throw std::runtime_error{"invalid division"};
			}
			if (rem.is_zero()) {
				continue;
			}
			if (leading_term_in(rem, polynomials)) {
				// TODO: remove existing ones...
				continue;
			}

			outf << "Found: (" << count++ << ")" << std::endl;
			outf << rem << std::endl;
//			if (count > 27) {
//				return;
//			}
			int index = (int) polynomials.size();
			add_all_less(queue, index);
			polynomials.push_back(rem);
		}
	}
}

#endif //IDEA_GROEBNER_H
