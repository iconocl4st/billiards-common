//
// Created by thallock on 10/13/21.
//

#ifndef IDEA_F4_H
#define IDEA_F4_H

#include <list>

#include "algebra/alg/F4State.h"
#include "algebra/alg/PolyMat.h"
#include "algebra/alg/reduce_grobner.h"

namespace algebra::poly {

	inline
	void collect_terms(const PolyPtr& poly, std::set<IndexPtr, MonomialOrder>& terms) {
		for (const auto& term : poly->terms) {
			terms.insert(term.first);
		}
	}

	inline
	void collect_terms(const std::vector<PolyPtr>& polys, std::set<IndexPtr, MonomialOrder>& terms) {
		for (const auto& poly : polys) {
			collect_terms(poly, terms);
		}
	}

	inline
	void construct_sygyzys(F4State& state, std::list<Pair>& B, std::vector<PolyPtr>& sygyzys) {
		for (const auto& pair : B) {
			const auto& fi = state.polys[pair.idx1];
			const auto& fj = state.polys[pair.idx2];
			const auto& fil = fi->leading().first;
			const auto& fjl = fj->leading().first;
			sygyzys.push_back(fi * Monomial{fil->lcm(fjl)->div(fil), 1.0});
			sygyzys.push_back(fj * Monomial{fjl->lcm(fil)->div(fjl), 1.0});
		}
	}

	inline
	void construct_H(
		F4State& state,
		std::vector<PolyPtr>& sygyzys,
		std::set<IndexPtr, MonomialOrder>& terms,
		std::ostream& os
	) {
		collect_terms(sygyzys, terms);

		const auto cmp = state.cmp();
		std::set<IndexPtr, MonomialOrder> done{cmp};
		for (const auto& poly : sygyzys) {
			done.insert(poly->leading().first);
		}
		while (true) {
			os << "constructing h: done  = ";
			for (const auto& m : done) {
				os << *m << " ";
			}
			os << "\n";
			os << "constructing h: terms = ";
			for (const auto& m : terms) {
				os << *m << " ";
			}
			os << "\n";

			if (done.size() >= terms.size()) {
				break;
			}

			IndexPtr max_index = sygyzys[0]->index_impl->create_empty();
			for (const auto& m : terms) {
				const auto& it = done.find(m);
				if (it != done.end()) {
					continue;
				}
				if (cmp(max_index, m)) {
					max_index = m;
				}
			}
			done.insert(max_index);
			os << "constructing h: found maximum degree: " << *max_index << "\n";

			for (const auto& fl : state.polys) {
				const auto& leading = fl->leading().first;
				if (!leading->divides(max_index)) {
					continue;
				}
				const PolyPtr h = fl * Monomial{max_index->div(leading), 1.0};
				sygyzys.push_back(h);
				collect_terms(h, terms);

				os << "constructing h: \tfound polynomial f_l = " << fl << "\n";
				os << "constructing h: \tComputed h = " << h << "\n";
				break;
			}
		}
	}

	[[nodiscard]] inline
	bool leading_is_generated_by(const PolyPtr& poly, std::vector<PolyPtr>& sygyzys) {
		std::set<IndexPtr, MonomialOrder> lms{poly->index_impl->get_comparator()}; // Is this already calculated?
		for (const auto& p : sygyzys) {
			lms.insert(p->leading().first);
		}
//		std::cout << "leading(H):" << std::endl;
//		for (const auto& p : lms) {
//			std::cout << "\t" << *p << std::endl;
//		}

		const auto& leading = poly->leading().first;
		for (const auto& leading_h : lms) {
			if (leading_h->divides(leading)) {
				return true;
			}
		}
		return false;
	}

	[[nodiscard]] inline
	std::vector<PolyPtr> f4(std::vector<PolyPtr>& polys, const std::function<void(const PolyPtr&)>& listener) {
		F4State state{};
		for (const auto& poly : polys) {
			state.add_polynomial(poly->to_monic());
		}

		std::ofstream outstream{"f4_log.txt"};

		while (!state.queue.empty()) {
			outstream << "======================================" << "\n";
			outstream << "State:" << "\n";
			outstream << state << "\n";

			std::list<Pair> B;
			select_subset(state, B);
			outstream << "Selected B = ";
			for (const auto& b : B) {
				outstream << b << " ";
			}
			outstream << "\n";

			std::vector<PolyPtr> sygyzys;
			construct_sygyzys(state, B, sygyzys);
			outstream << "Constructed sygyzys:" << "\n";
			for (const auto& s : sygyzys) {
				outstream << "\t" << s << "\n";
			}
			
			for (const auto& poly : sygyzys) {
				listener(poly); // test it...
			}

			std::set<IndexPtr, MonomialOrder> terms{state.cmp()};
			construct_H(state, sygyzys, terms, outstream);

			for (const auto& poly : sygyzys) {
				listener(poly); // test it...
			}

			outstream << "Computed H:" << "\n";
			for (const auto& p : sygyzys) {
				outstream << "\t" << p << "\n";
			}

			outstream << "terms: ";
			for (const auto& t : std::ranges::reverse_view(terms)) {
				outstream << *t << " ";
			}
			outstream << std::endl;

#if 0
			PolyDenseMat mat{terms, sygyzys};
#else
			PolySparseMat mat{terms, sygyzys};
#endif

//			Eigen::MatrixXd M = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>{sygyzys.size(), terms.size()};

			outstream << "Computed M:" << "\n";
			outstream << mat.M.toDense() << "\n";


			for (int i = 0; i < (int) sygyzys.size(); i++) {
				const auto& poly = mat.get_poly(i);
				listener(poly);
			}

			mat.reduce();

			outstream << "Reduced M:" << "\n";
			outstream << mat.M.toDense() << "\n";

			for (int i = 0; i < (int) sygyzys.size(); i++) {
				const auto& poly = mat.get_poly(i);
				outstream << "Checking " << poly << " ..." << "\n";
				if (poly->is_zero()) {
					outstream << "\t\t...is zero" << "\n";
					continue;
				}
				if (leading_is_generated_by(poly, sygyzys)) {
					outstream << "\t\t..." << poly->leading() << " is generated" << "\n";
					continue;
				}
				outstream << "\t\t...adding." << "\n";

				outstream << "Adding " << poly << "\n";
				state.add_polynomial(poly);

				outstream.flush();
				listener(poly);
			}

			outstream << std::endl;
		}

//		reduce_grobner(state.polys);
		return state.polys;
	}


	[[nodiscard]] inline
	std::vector<PolyPtr> f4(std::vector<PolyPtr>& polys) {
		return f4(polys, [](const PolyPtr&){});
	}
}

#endif //IDEA_F4_H
