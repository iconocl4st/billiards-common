//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_CHECK_GROBNER_H
#define IDEA_CHECK_GROBNER_H

#include "algebra/poly_divide.h"

namespace algebra::poly {
	bool is_grobner(std::vector<PolyPtr>& basis) {
		for (int i = 0; i < basis.size(); i++) {
			for (int j = 0; j < basis.size(); j++) {
				if (i == j) {
					continue;
				}
			}
		}
		return true;
	}
}

#endif //IDEA_CHECK_GROBNER_H
