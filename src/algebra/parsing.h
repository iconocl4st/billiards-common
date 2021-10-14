//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_PARSING_H
#define IDEA_PARSING_H


#include <regex>

#include "algebra/Polynomial.h"

namespace algebra::poly::parsing {

	void parse_powers(std::string& str, Monomial& m) {
		std::stringstream s;
		std::regex_token_iterator<std::string::iterator> rend;

		const std::regex expr{"\\s**?\\s*x\\[\\s*(\\d+)\\s*\\](\\^(\\d+))?"};
		int submatches[] = {1, 3};
		std::regex_token_iterator<std::string::iterator> it{
			str.begin(), str.end(), expr, submatches};
		while (it != rend) {
			std::string index_s = *it++;
			std::string power_s = *it++;

//			std::cout << "\t\tFound index: " << index_s << std::endl;
//			std::cout << "\t\tFound power: " << power_s << std::endl;

			int power;
			if (power_s.empty()) {
				power = 1;
			} else {
				power = std::stoi(power_s);
			}

			int index = std::stoi(index_s);
			if (index < 0 || index >= m.dim()) {
				throw std::runtime_error{"Invalid variable"};
			}
			m.powers[index] = power;
		}
	}

	Monomial parse_monomial(std::string& str, int dim) {
		std::stringstream s;
		std::regex_token_iterator<std::string::iterator> rend;

//		std::cout << "\tSearching " << str << std::endl;

		const std::regex expr{"\\s*(\\d+)?\\s*\\*?\\s*(x[\\sx\\[\\]^*\\d]*)?"}; // only supports ints
		int submatches[] = {1, 2};
		std::regex_token_iterator<std::string::iterator> it{
			str.begin(), str.end(), expr, submatches};

		if (it == rend) {
			return Monomial{dim};
		}
		std::string coefficient = *it++;
		std::string powers = *it++;

//		std::cout << "\tFound coefficient: " << coefficient << std::endl;
//		std::cout << "\tFound powers: " << powers << std::endl;

		Monomial m{dim};
		if (coefficient.empty()) {
			m.coefficient = 1.0;
		} else {
			m.coefficient = std::stod(coefficient);
		}
		if (!powers.empty()) {
			parse_powers(powers, m);
		}
		return m;
	}

	Polynomial parse_polynomial(std::string& str, int dim) {
		Polynomial p{dim};
		std::stringstream s;
		std::regex_token_iterator<std::string::iterator> rend;

		int submatches[] = {1, 2};
		const std::regex expr{"\\s*([+-]?)\\s*([^-+]+)"};
		std::regex_token_iterator<std::string::iterator> it{
			str.begin(), str.end(), expr, submatches};
		while (it != rend) {
			std::string sign = *it++;
			std::string term = *it++;

//			std::cout << "Found sign: " << sign << std::endl;
//			std::cout << "Found term: " << term << std::endl;

			Monomial m = parse_monomial(term, dim);
			if (sign == "-") {
				m.coefficient *= -1;
			}
			p += m;
		}
		return p;
	}
}

#endif //IDEA_PARSING_H
