//
// Created by thallock on 10/21/21.
//

#ifndef IDEA_OLD_H
#define IDEA_OLD_H


#if 0

bool leading_is_generated_by(const MultiIndex& leading, std::vector<MultiIndex>& indices, int index) {
	if (leading.is_constant()) {
		return true;
	}
	if (index >= (int) indices.size()) {
		return false;
	}
	MultiIndex r = leading;
	while (true) {
		if (leading_is_generated_by(r, indices, index + 1)) {
			return true;
		}
		if (!indices[index].divides(r)) {
			return false;
		}
		r = r / indices[index];
	}
}

bool leading_is_generated_by(const PolyDict& poly, std::vector<PolyDict>& sygyzys) {
	std::set<MultiIndex> lms; // Is this already calculated?
	for (const auto& p : sygyzys) {
		lms.insert(p.leading());
	}
	std::cout << "leading(H):" << std::endl;
	for (const auto& p : lms) {
		std::cout << "\t" << p << std::endl;
	}
	std::vector<MultiIndex> lmv{lms.begin(), lms.end()};

	return leading_is_generated_by(poly.leading(), lmv, 0);
}
#endif

#endif //IDEA_OLD_H
