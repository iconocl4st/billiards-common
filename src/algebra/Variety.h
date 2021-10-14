//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_VARIETY_H
#define IDEA_VARIETY_H

#include "algebra/Polynomial.h"

namespace algebra::poly {

	class Variety {
	public:
		std::vector<Polynomial> polynomials;
		// include the names here
		// include a powers cache...

		Variety() = default;
		~Variety() = default;
//		Variety(const std::vector<Polynomial>& polynomials) : polynomials{polynomials} {}


		Variety substitute(int poly_idx, int var_idx, const Polynomial& expr) {
			Variety ret = Variety{};
			for (int i = 0; i < (int) polynomials.size(); i++) {
				if (i == poly_idx) {
					continue;
				}
				ret.polynomials.push_back(polynomials[i].substitute(var_idx, expr).canon());
			}
			return ret;
		}

		void simplify() {
			for (const Polynomial& poly : polynomials) {
				for (int i = 0; i < poly.dim(); i++) {
//					if (simplify::poly_has_assignment_var(poly, i)) {
//						std::cout << "Can use " << poly << " to assign " << i << std::endl;
//					}
				}
			}

			for (const Polynomial& poly : polynomials) {
				for (int i = 0; i < poly.dim(); i++) {
					if (simplify::poly_is_linear_in(poly, i)) {
						std::cout << "Can use " << poly << " to solve for " << i << std::endl;
					}
				}
			}
		}
	};

	namespace simplify {
		Variety simplify_variety(const Variety& variety) {
			Variety ret = variety;
			int count = 0;
			bool simplified = true;
			while (simplified && count++ < 7) {
				simplified = false;
				for (int p = 0; p < (int) ret.polynomials.size() && !simplified; p++) {
					const Polynomial& poly = ret.polynomials[p];
					for (int v = 0; v < poly.dim() && !simplified; v++) {
						int term_index = simplify::get_assignment_var(poly, v);
						if (term_index < 0) {
							continue;
						}

						const Monomial& m = poly.terms[term_index];
						const auto expr = -poly.without(term_index) / m.coefficient;
						std::cout << "Can use " << poly << " to assign " << v << std::endl;
						std::cout << "Substituting " << v << " = " << expr << std::endl;

						ret = ret.substitute(p, v, expr);
						simplified = true;
					}
				}
			}
			return ret;
		}
	}
}

#endif //IDEA_VARIETY_H
