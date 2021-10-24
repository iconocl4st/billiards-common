//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_VARIETY_H
#define IDEA_VARIETY_H

#include "algebra/PolyDict.h"

namespace algebra::poly {

//	class Variety {
//	public:
//
//		std::vector<PolyVec> polynomials;
//
//		Variety() = default;
//		~Variety() = default;
////		Variety(const std::vector<PolyVec>& polynomials) : polynomials{polynomials} {}
//
//
//		Variety substitute(int poly_idx, int var_idx, const PolyVec& expr) {
//			Variety ret = Variety{};
//			for (int i = 0; i < (int) polynomials.size(); i++) {
//				if (i == poly_idx) {
//					continue;
//				}
//				ret.polynomials.push_back(polynomials[i].substitute(var_idx, expr).canon());
//			}
//			return ret;
//		}
//
//		void simplify() {
//			for (const PolyVec& poly : polynomials) {
//				for (int i = 0; i < poly.dim(); i++) {
////					if (simplify::poly_has_assignment_var(poly, i)) {
////						std::cout << "Can use " << poly << " to assign " << i << std::endl;
////					}
//				}
//			}
//
//			for (const PolyVec& poly : polynomials) {
//				for (int i = 0; i < poly.dim(); i++) {
//					if (simplify::poly_is_linear_in(poly, i)) {
//						std::cout << "Can use " << poly << " to solve for " << i << std::endl;
//					}
//				}
//			}
//		}
//	};

	namespace simplify {


		std::vector<PolyPtr> simplify_linear_expressions(std::vector<PolyPtr>& polys) {
			bool simplified = true;
			std::vector<PolyPtr> curr = polys;
			int count = 0;
			while (simplified && count++ < 400) {
				simplified = false;
				for (int p = 0; p < (int) curr.size() && !simplified; p++) {
					const auto& poly = curr[p];
					for (int v = 0; v < poly->index_impl->ideal->dim() && !simplified; v++) {
						auto var = simplify::get_assignment_var(poly, v);
						if (!var) {
							continue;
						}

						std::cout << "Can use " << poly << " to assign " << poly->index_impl->ideal->name(v) << std::endl;
						std::cout << "Substituting " << poly->index_impl->ideal->name(v) << " = " << *var << std::endl;

						std::vector<PolyPtr> reduced_system;
						for (int i = 0; (int) i < curr.size(); i++) {
							if (i == p) {
								continue;
							}
							reduced_system.push_back(curr[i]->substitute(v, *var));
						}
						curr = reduced_system;
						simplified = true;
					}
				}
			}
			return curr;
		}
		std::vector<PolyPtr> divide_by_gcd(std::vector<PolyPtr>& polys) {
			to_reduced(polys);
			for (int i = 0; i < polys.size(); i++) {
			}
			return polys;
		}

		std::vector<PolyPtr> simplify_variety(std::vector<PolyPtr>& polys) {
			polys = simplify_linear_expressions(polys);
			polys = divide_by_gcd(polys);
			return polys;
		}
	}
}

#endif //IDEA_VARIETY_H
