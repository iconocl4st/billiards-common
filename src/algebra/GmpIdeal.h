//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_GMPIDEAL_H
#define IDEA_GMPIDEAL_H

#include "algebra/Ideal.h"

namespace algebra::poly {

	class GmpIdeal : public Ideal {
	public:
		int log_2_max_deg;

		explicit GmpIdeal(cmp::order order) : Ideal(order), log_2_max_deg{8} {};

		~GmpIdeal() override = default;
	};
}

#endif //IDEA_GMPIDEAL_H
