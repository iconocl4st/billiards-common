//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_PARSING_H
#define IDEA_PARSING_H

#include <regex>

#include "MultiIndexImpl.h"
#include "PolyDict.h"

namespace algebra::poly::parsing {

	// TODO: combine this...
	inline
	void parse_powers(std::string& str, const std::shared_ptr<MultiIndex>& m) {
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
			if (index < 0 || index >= m->dim()) {
				throw std::runtime_error{"Invalid variable"};
			}
			m->set(index, power);
		}
	}

	[[nodiscard]] inline
	std::optional<Monomial> parse_monomial(const std::shared_ptr<IndexImpl>& impl, std::string& str) {
		std::stringstream s;
		std::regex_token_iterator<std::string::iterator> rend;

//		std::cout << "\tSearching " << str << std::endl;

		const std::regex expr{"\\s*(\\d+)?\\s*\\*?\\s*(x[\\sx\\[\\]^*\\d]*)?"}; // only supports ints
		int submatches[] = {1, 2};
		std::regex_token_iterator<std::string::iterator> it{
			str.begin(), str.end(), expr, submatches};

		if (it == rend) {
			return {};
		}
		std::string coefficient = *it++;
		std::string powers = *it++;

//		std::cout << "\tFound coefficient: " << coefficient << std::endl;
//		std::cout << "\tFound powers: " << powers << std::endl;

		Monomial m{impl->create_empty(), coefficient.empty() ? 1.0 : std::stod(coefficient)};
		if (!powers.empty()) {
			parse_powers(powers, m.first);
		}
		return m;
	}

	[[nodiscard]] inline
	PolyPtr parse_polynomial(const std::shared_ptr<IndexImpl>& impl, const std::string& const_str) {
		std::string str{const_str};
		PolyPtr p = std::make_shared<PolyDict>(impl);

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

			auto m = parse_monomial(impl, term);
			if (!m) {
				continue;
			}
			if (sign == "-") {
				m->second *= -1;
			}
			p->operator+=(m.value());
		}
		return p;
	}
}

#endif //IDEA_PARSING_H
