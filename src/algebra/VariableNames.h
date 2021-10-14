//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_VARIABLENAMES_H
#define IDEA_VARIABLENAMES_H

#include <vector>

#include "algebra/Polynomial.h"

namespace algebra::poly {

	class VariableNames {
	public:
		std::vector<std::string> names;

		VariableNames() : names{} {}
		~VariableNames() = default;

		[[nodiscard]] inline
		int dim() const {
			return (int) names.size();
		}

		[[nodiscard]] inline
		int get_var(const std::string& name) const {
			for (int i = 0; i < (int) names.size(); i++) {
				if (names[i] == name) {
					return i;
				}
			}
			return -1;
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const VariableNames& n) {
			for (int i = 0; i < (int) n.names.size(); i++) {
				if (i != 0) {
					os << ", ";
				}
				os << n.names[i] << "=x[" << i << "]";
			}
			return os;
		}

		[[nodiscard]] inline
		Polynomial operator()(const std::string& name) const {
			int index = get_var(name);
			Monomial m{dim()};
			m.coefficient = 1.0;
			m.powers[index] = 1;
			Polynomial p{dim()};
			p += m;
			return p;
		}
		[[nodiscard]] inline
		Formatter create_formatter() const {
			return [&](int index) {
				return names[index];
			};
		}

		[[nodiscard]] inline
		PolynomialWithFormatter operator()(const Polynomial& p) const {
			return PolynomialWithFormatter{p, create_formatter()};
		}

		[[nodiscard]] inline
		std::string get_x(const std::string& name) {
			std::stringstream ss;
			ss << "x[" << get_var(name) << "]";
			return ss.str();
		}

		inline
		int register_var(const std::string& name) {
			int idx = get_var(name);
			if (idx >= 0) {
				return idx;
			}
			idx = (int) names.size();
			names.push_back(name);
			return idx;
		}
	};
}
#endif //IDEA_VARIABLENAMES_H
