//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_POLY_DIVIDE_H
#define IDEA_POLY_DIVIDE_H

#include "algebra/PolyDict.h"

namespace algebra::poly {

	class Division {
	public:
		std::vector<PolyPtr> divisors;
		PolyPtr dividend;

		std::vector<PolyPtr> quotients;
		PolyPtr remainder;
		int ndiv;

		explicit Division(PolyPtr& dividend)
			: divisors{}
			, dividend{dividend}
			, quotients{}
			, remainder{dividend->empty()}
			, ndiv{0}
		{}

		~Division() = default;

		inline
		void add_divisor(const PolyPtr& ptr) {
			divisors.emplace_back(ptr);
			quotients.push_back(ptr->empty());
			ndiv++;
		}

		[[nodiscard]] inline
		bool check() const {
//			std::cout << "Need: \n";
//			std::cout << dividend << " = " << remainder << "\n";
			for (int i = 0; i < ndiv; i++) {
				if (quotients[i]->is_zero()) {
					continue;
				}
//				std::cout << " + (" << quotients[i] << ") * (" << divisors[i] << ")";
			}

			PolyPtr p = remainder->copy();
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

			double check_tol = 1e-5;
			if (!p->is_zero(check_tol)) {
				std::cout << "Was supposed to be zero:" << std::endl;
				std::cout << p << std::endl;
			}

			return p->is_zero(check_tol);
		}
	};

	inline
	void divide(Division& d) {
		PolyPtr tmp = d.dividend->copy();
		while (!tmp->is_zero()) {
			bool divided = false;
			const auto& leading_d = tmp->leading();

//			std::cout << "tmp: " << std::endl;
//			std::cout << tmp << std::endl;
//
//			std::cout << "leading: " << std::endl;
//			std::cout << leading_d << std::endl;
			for (int i = 0; i < d.ndiv && !divided; i++) {
				if (d.divisors[i]->is_zero()) {
					throw std::runtime_error{"It wasn't zero!"};
				}
				const auto& leading_q = d.divisors[i]->leading();
				if (!leading_q.first->divides(leading_d.first)) {
					continue;
				}
//				std::cout << "Found " << i << " " << *leading_q.first << std::endl;
				const auto q = Monomial(leading_d.first->div(leading_q.first), leading_d.second / leading_q.second);
//				std::cout << "q: " << q << std::endl;
				d.quotients[i] += q;
//				std::cout << "subtracting " << (d.divisors[i] * q) << std::endl;
				tmp -= d.divisors[i] * q;
				divided = true;
			}
			if (divided) {
				continue;
			}
			d.remainder += leading_d;
			tmp->erase(leading_d.first);
		}
	}
}

#endif //IDEA_POLY_DIVIDE_H
