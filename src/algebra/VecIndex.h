//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_VECINDEX_H
#define IDEA_VECINDEX_H

#include "algebra/Index.h"

#include <iostream>
#include <utility>


namespace algebra::poly {

	class VectorIndex : public MultiIndex {
	public:
		std::vector<int> powers;

		VectorIndex(const std::shared_ptr<Ideal>& ideal, std::vector<int> powers)
			: MultiIndex(ideal)
			, powers(std::move(powers))
		{}

		explicit VectorIndex(const std::shared_ptr<Ideal>& ideal)
			: MultiIndex(ideal)
			, powers{/*ideal->dim(), 0*/}
		{
			powers.reserve(ideal->dim());
			for (int i = 0; i < ideal->dim(); i++) {
				powers.push_back(0);
			}
//			std::cout << "Created with powers: " << std::endl;
//			std::cout << powers[0] << std::endl;
//			std::cout << powers[1] << std::endl;
		}

		[[nodiscard]] inline
		int get(int var_index) const override {
			return powers[var_index];
		}
		inline
		void set(int var_index, int power) override {
			powers[var_index] = power;
		}

		[[nodiscard]] inline
		std::shared_ptr<MultiIndex> copy() const override {
			return std::make_shared<VectorIndex>(ideal, powers);
		}

		[[nodiscard]] inline
		std::shared_ptr<MultiIndex> empty() const override {
			return std::make_shared<VectorIndex>(ideal);
		};
	};
}
#endif //IDEA_VECINDEX_H
