//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_POLY_DIVIDE_H
#define IDEA_POLY_DIVIDE_H

#include "Polynomial.h"

namespace algebra::poly {

	class Division {
	public:
		const std::vector<Polynomial>& divisors;
		const Polynomial& dividend;

		std::vector<Polynomial> quotients;
		Polynomial remainder;
		int ndiv;

		Division(const std::vector<Polynomial>& divisors, const Polynomial& dividend)
			: divisors{divisors}
			, dividend{dividend}
			, quotients{}
			, remainder{dividend.dim()}
			, ndiv{(int) divisors.size()}
		{
			for (int i = 0; i < ndiv; i++) {
				quotients.emplace_back(dividend.dim());
			}
		}

		[[nodiscard]] inline
		bool check() const {
//			std::cout << "Need: \n";
//			std::cout << dividend << " = " << remainder;
//			for (int i = 0; i < ndiv; i++) {
//				if (quotients[i].is_zero()) {
//					continue;
//				}
//				std::cout << " + (" << quotients[i] << ") * (" << divisors[i] << ")";
//			}

			Polynomial p = remainder;
			for (int i = 0; i < ndiv; i++) {
//				std::cout << "p" << std::endl;
//				std::cout << p << std::endl;

				p += quotients[i] * divisors[i];
			}
//			std::cout << "p" << std::endl;
//			std::cout << p << std::endl;

			p -= dividend;

//			std::cout << "p" << std::endl;
//			std::cout << p.collect() << std::endl;

			return p.is_zero();
		}
	};

	void divide(Division& d) {
		Polynomial tmp = d.dividend;
		while (!tmp.conan_is_zero()) {
			bool divided = false;

			if (tmp.terms.empty()) {
				throw std::runtime_error{"It wasn't zero!"};
			}
			const auto& leading_d = tmp.terms[0];

//			std::cout << "tmp: " << std::endl;
//			std::cout << tmp << std::endl;
			for (int i = 0; i < d.ndiv && !divided; i++) {
				if (d.divisors[i].terms.empty()) {
					throw std::runtime_error{"It wasn't zero!"};
				}
				const auto& leading_q = d.divisors[i].terms[0];
				if (!leading_q.divides(leading_d)) {
					continue;
				}
//				std::cout << "Found " << i << " " << leading_q << std::endl;
				const auto q = leading_d / leading_q;
//				std::cout << "q: " << q << std::endl;
				d.quotients[i] += q;
				tmp -= d.divisors[i] * q;
				tmp = tmp.collect().nonzero();
				divided = true;
			}
			if (divided) {
				continue;
			}
			d.remainder += leading_d;
			tmp.erase_first();
		}
	}
}

#endif //IDEA_POLY_DIVIDE_H
