//
// Created by thallock on 10/21/21.
//

#ifndef IDEA_F4STATE_H
#define IDEA_F4STATE_H

#include <set>
#include <map>
#include <climits>

#include <Eigen/Dense>

#include "algebra/PolyDict.h"
#include "algebra/alg/Pair.h"

namespace algebra::poly {

	class F4State {
	public:
		std::vector<PolyPtr> polys;
		std::map<Pair, int> degrees;
		std::set<Pair> queue;

		explicit F4State()
			: polys{}
			, degrees{}
			, queue{}
		{}

		void add_polynomial(const PolyPtr& poly) {
			int poly_index = (int) polys.size();
			for (int i = 0; i < poly_index; i++) {
				queue.emplace(i, poly_index);
			}

			const auto& leading1 = poly->leading().first;
			for (int i = 0; i < (int) polys.size(); i++) {
				const auto& leading2 = polys[i]->leading().first;
//				std::cout << "Computing deg of pair " << i << "," << poly_index << std::endl;
//				std::cout << "\tleading 1 = " << *leading1 << std::endl;
//				std::cout << "\tleading 2 = " << *leading2 << std::endl;
//				std::cout << "\tlcm = " <<  *leading1->lcm(leading2) << std::endl;
//				std::cout << "\tdegree = " << leading1->lcm(leading2)->degree() << std::endl;
				degrees[Pair{i, poly_index}] = leading1->lcm(leading2)->degree();
			}
			polys.push_back(poly);
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const F4State& state) {
			os << "Current basis:" << "\n";
			for (const auto& poly : state.polys) {
				os << "\t" << poly << "\n";
			}
//			os << "Degree of pairs:" << std::endl;
//			for (const auto& deg : state.degrees) {
//				os << "\t" << deg.first << " -> " << deg.second << std::endl;
//			}
			os << "Queue:" << "\n";
			for (const auto& p : state.queue) {
				os << "\t" << p << " (deg=" << state.degrees.find(p)->second << ")" << "\n";
			}
			return os;
		}

		[[nodiscard]] inline
		MonomialOrder cmp() const {
			return polys[0]->index_impl->get_comparator();
		}
	};


	inline
	void select_subset(F4State& state, std::list<Pair>& B) {
		int m = INT_MAX;
		for (const auto& pair : state.queue) {
			auto degree = state.degrees.find(pair);
			if (degree == state.degrees.end()) {
				throw std::runtime_error{"Failed to compute the degree of a pair"};
			}
			if (degree->second < m) {
				B.clear();
				m = degree->second;
			}
			if (degree->second == m) {
				B.push_back(degree->first);
			}
		}
		for (const auto& pair : B) {
			auto it = state.queue.find(pair);
			if (it == state.queue.end()) {
				throw std::runtime_error{"We just put it here..."};
			}
			state.queue.erase(it);
		}
	}


}

#endif //IDEA_F4STATE_H
