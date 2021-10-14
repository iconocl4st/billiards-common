//
// Created by thallock on 10/13/21.
//

#ifndef IDEA_GMPPOLY_H
#define IDEA_GMPPOLY_H

#include <gmpxx.h>
#include <gmp.h>

#include <utility>
#include <vector>
#include <map>
#include <regex>
#include <sstream>
#include <iostream>

namespace algebra::poly {
#define YET_ANOTHER_TOL (1e-10)

//	mpz_class get_max_deg(int log_2_max_deg) {
//		mpz_t two;
//		mpz_init2(two, 2);
//		mpz_set_ui(two, 2);
//
//		mpz_t max_deg;
//		mpz_init2(max_deg, log_2_max_deg + 1);
////		mpz_mul_2exp(max_deg, two, log_2_max_deg);
//		mpz_pow_ui(max_deg, two, log_2_max_deg);
//
//		return mpz_class{max_deg};
//	}

	class Ideal {
	public:
		std::vector<std::string> names;
//		std::vector<mpz_class> bases;
		int log_2_max_deg;
//		mpz_class max_deg;

		Ideal()
			: names{}
//			, bases{}
			, log_2_max_deg{8}
//			, max_deg{get_max_deg(log_2_max_deg)}
		{
		};

		~Ideal() = default;

		[[nodiscard]] inline
		int dim() const {
			return (int) names.size();
		}

		[[nodiscard]] inline
		const std::string& name(int index) const {
			return names[index];
		}

		inline
		void register_var(const std::string& name) {
			for (const auto& prev: names) {
				if (name == prev) {
					return;
				}
			}
//			mpz_t b;
//			mpz_init_set_ui(b, 1);
//			mpz_mul_2exp(b, b, dim() * log_2_max_deg);
//			bases.emplace_back(b);
			names.push_back(name);
		}
	};

	class MultiIndex {
	public:
		const Ideal& ideal;
		mpz_t powers;

		MultiIndex(const Ideal& ideal, const mpz_t& p)
			: ideal{ideal}, powers{} {
			mpz_init_set(powers, p);
		}

		MultiIndex(const MultiIndex& m)
			: ideal{m.ideal}, powers{} {
			mpz_init_set(powers, m.powers);
		}

		explicit MultiIndex(const Ideal& ideal)
			: ideal{ideal}, powers{} {
			mpz_init2(powers, ideal.dim() * ideal.log_2_max_deg);
		}

		~MultiIndex() {
			mpz_clear(powers);
		}

		inline
		void set(int var, int power) {
			mpz_t term;
			mpz_init_set_ui(term, power);
			mpz_mul_2exp(term, term, var * ideal.log_2_max_deg);
			mpz_add(powers, powers, term);
		}

		[[nodiscard]] inline
		int get(int var) const {
			mpz_t ret;
			mpz_init_set(ret, powers);
			mpz_div_2exp(ret, ret, var * ideal.log_2_max_deg);
			mpz_mod_2exp(ret, ret, ideal.log_2_max_deg);
			return mpz_get_ui(ret);
		}

		[[nodiscard]] inline
		int dim() const {
			return ideal.dim();
		}

		[[nodiscard]] inline
		double evaluate(const std::vector<double>& x) const {
			int d = dim();
			double value = 1.0;
			for (int i = 0; i < d; i++) {
				int p = get(i);
				if (p == 0) {
					continue;
				}
				value *= std::pow(x[i], p);
			}
			return value;
		}

		[[nodiscard]] inline
		bool divides(const MultiIndex& other) const {
			int d = dim();
			for (int i = 0; i < d; i++) {
				if (get(i) > other.get(i)) {
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] inline
		bool is_divisible_by(const MultiIndex& other) const {
			return other.divides(*this);
		}


		[[nodiscard]] inline
		MultiIndex lcm(const MultiIndex& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot compute lcm in different dimensions"};
			}
			MultiIndex ret{ideal};
			for (int i = 0; i < ideal.dim(); i++) {
				ret.set(i, std::max(get(i), other.get(i)));
			}
			return ret;
		}
//
//
//		[[nodiscard]] inline
//		int degree(int idx) const {
////			if (idx < 0 || idx >= powers.size()) {
////				throw std::runtime_error{};
////			}
//			return powers[idx];
//		}
//
		[[nodiscard]] inline
		int degree() const {
			int sum = 0;
			for (int i=0; i<ideal.dim(); i++) {
				sum += get(i);
			}
			return sum;
		}

		[[nodiscard]] inline
		bool is_constant() const {
			return mpz_sgn(powers) == 0;
		}

		inline
		MultiIndex& operator=(const MultiIndex& other) {
			mpz_set(powers, other.powers);
			return *this;
		}

		[[nodiscard]] inline
		bool operator==(const MultiIndex& other) const {
			return mpz_cmp(powers, other.powers) == 0;
		}

		[[nodiscard]] inline
		bool operator<(const MultiIndex& other) const {
			return mpz_cmp(powers, other.powers) < 0;
		}

		[[nodiscard]] inline
		bool operator<=(const MultiIndex& other) const {
			return mpz_cmp(powers, other.powers) <= 0;
		}

		[[nodiscard]] inline
		bool operator>(const MultiIndex& other) const {
			return mpz_cmp(powers, other.powers) > 0;
		}

		[[nodiscard]] inline
		bool operator>=(const MultiIndex& other) const {
			return mpz_cmp(powers, other.powers) >= 0;
		}

//
		inline
		friend std::ostream& operator<<(std::ostream& os, const MultiIndex& m) {
//			os << "(" << m.powers << ") ";
			if (m.is_constant()) {
				return os;
			}

			bool requires_multiplication = false;
			for (int i = 0; i < m.ideal.dim(); i++) {
				const int p = m.get(i);
				if (p == 0) {
					continue;
				}
				if (requires_multiplication) {
					os << "*";
				} else {
					requires_multiplication = true;
				}
				os << m.ideal.name(i);
				if (p != 1) {
					os << "^" << p;
				}
			}
			return os;
		}

		[[nodiscard]] inline
		MultiIndex operator*(const MultiIndex& other) const {
			MultiIndex ret{ideal};
			mpz_add(ret.powers, powers, other.powers);
			return ret;
		}

		[[nodiscard]] inline
		MultiIndex operator/(const MultiIndex& other) const {
			if (!is_divisible_by(other)) {
				throw std::runtime_error{"Not divisible"};
			}
			MultiIndex ret{ideal};
			mpz_sub(ret.powers, powers, other.powers);
			return ret;
		}
	};


	class PolyDict {
	public:
		const Ideal& ideal;
		std::map<const MultiIndex, double> terms;

		explicit PolyDict(const Ideal& ideal) : ideal{ideal}, terms{} {}

		PolyDict(const Ideal& ideal, std::map<const MultiIndex, double> terms)
			: ideal{ideal}, terms{std::move(terms)} {}

		PolyDict(const PolyDict& other) : PolyDict{other.ideal, other.terms} {}

		~PolyDict() = default;

		[[nodiscard]] inline
		PolyDict substitute(int index, const PolyDict& value) {
			PolyDict p{ideal}; // Could reduce the dimension...
			for (const auto& term: terms) {
				const int old_power = term.first.get(index);
				if (old_power == 0) {
					p += term;
					continue;
				}
				MultiIndex rest{term.first};
				rest.set(index, 0);
				p += value.pow(old_power) * std::pair<MultiIndex, double>{rest, term.second};
			}
			return p;
		}

		[[nodiscard]] inline
		double evaluate(const std::vector<double>& x) const {
			double value = 0;
			for (const auto& term: terms) {
				value += term.first.evaluate(x) * term.second;
			}
			return value;
		}

		[[nodiscard]] inline
		int dim() const {
			return ideal.dim();
		}

		[[nodiscard]] inline
		int degree() const {
			int deg = -1;
			for (const auto& term : terms) {
				deg = std::max(deg, term.first.degree());
			}
			return deg;
		}

		[[nodiscard]] inline
		const MultiIndex& leading() const {
			return terms.rbegin()->first;
		}

		[[nodiscard]] inline
		bool is_homogeneous() const {
			return false;
		}

		[[nodiscard]] inline
		PolyDict to_monic() const {
			return *this / terms.rbegin()->second;
		}

		[[nodiscard]] inline
		PolyDict without(const MultiIndex& term) const {
			PolyDict p = copy();
			p.terms.erase(term);
			return p;
		}

		[[nodiscard]] inline
		bool is_zero() const {
			return terms.empty();
		}

		[[nodiscard]] inline
		PolyDict copy() const {
			return PolyDict{ideal, terms};
		}

		[[nodiscard]] inline
		PolyDict pow(int power) const {
			if (power == 0) {
				PolyDict p{ideal};
				p.terms.emplace(ideal, 1.0);
				return p;
			}
			PolyDict p = copy();
			for (int i = 1; i < power; i++) {
				p = p * *this;
			}
			return p;
		}

		inline
		void set(const MultiIndex& index, const double value) {
			if (std::abs(value) < YET_ANOTHER_TOL) {
				terms.erase(index);
			} else {
				terms[index] = value;
			}
		}


//		[[nodiscard]] inline
//		PolyDict operator+(const PolyDict& other) const {
//			PolyDict p = copy();
//			for (const auto& t: other.terms) {
//				p += t;
//			}
//			return p;
//		}

		inline
		PolyDict& operator=(const PolyDict& other) {
			terms = other.terms;
			return *this;
		}

		////////////////////////////////////////////////////////
		/// Addition
		////////////////////////////////////////////////////////

		inline
		void operator+=(const std::pair<MultiIndex, double>& pair) {
			const auto& loc = terms.find(pair.first);
			if (loc == terms.end()) {
				set(pair.first, pair.second);
			} else {
				set(pair.first, loc->second + pair.second);
			}
		}

		inline
		PolyDict& operator+=(const PolyDict& m) {
			for (const auto& term: m.terms) {
				operator+=(term);
			}
			return *this;
		}

		[[nodiscard]] inline
		PolyDict operator+(const double v) const {
			PolyDict p = copy();
			p += std::pair<MultiIndex, double>{MultiIndex{ideal}, v};
			return p;
		}

		[[nodiscard]] inline
		PolyDict operator+(const PolyDict& other) const {
			PolyDict p = copy();
			p += other;
			return p;
		}

		////////////////////////////////////////////////////////
		/// Subtraction
		////////////////////////////////////////////////////////

		inline
		void operator-=(const std::pair<MultiIndex, double>& pair) {
			const auto& loc = terms.find(pair.first);
			if (loc == terms.end()) {
				set(pair.first, -pair.second);
			} else {
				set(pair.first, loc->second - pair.second);
			}
		}

		inline
		PolyDict& operator-=(const PolyDict& m) {
			for (const auto& term: m.terms) {
				operator-=(term);
			}
			return *this;
		}

		[[nodiscard]] inline
		PolyDict operator-(const PolyDict& other) const {
			PolyDict p = copy();
			for (const auto& t: other.terms) {
				p -= t;
			}
			return p;
		}

		[[nodiscard]] inline
		PolyDict operator-(const double v) const {
			PolyDict p = copy();
			p -= std::pair<MultiIndex, double>{MultiIndex{ideal}, v};
			return p;
		}

		[[nodiscard]] inline
		PolyDict operator-(const std::pair<MultiIndex, double>& other) const {
			PolyDict p = copy();
			p -= other;
			return p;
		}

		[[nodiscard]] inline
		PolyDict operator-() const {
			PolyDict p{ideal};
			for (const auto& term: terms) {
				p.terms.emplace(term.first, -term.second);
			}
			return p;
		}

		////////////////////////////////////////////////////////
		/// Multiplication
		////////////////////////////////////////////////////////

		[[nodiscard]] inline
		PolyDict operator*(const double v) const {
			PolyDict p{ideal};
			for (const auto& t: terms) {
				const double c = t.second * v;
				if (std::abs(c) < YET_ANOTHER_TOL) {
					continue;
				}
				p.terms.emplace(t.first, c);
			}
			return p;
		}

		[[nodiscard]] inline
		PolyDict operator*(const std::pair<MultiIndex, double>& pair) const {
			PolyDict ret = PolyDict{ideal};
			for (const auto& term: terms) {
				double c = term.second * pair.second;
				if (std::abs(c) < YET_ANOTHER_TOL) {
					continue;
				}
				ret.terms.emplace(term.first * pair.first, c);
			}
			return ret;
		}

		[[nodiscard]] inline
		PolyDict operator*(const MultiIndex& m) const {
			return *this * std::pair<MultiIndex, double>{m, 1.0};
		}

		[[nodiscard]] inline
		PolyDict operator*(const PolyDict& other) const {
			PolyDict ret = PolyDict{ideal};
			for (const auto& t1 : terms) {
				for (const auto& t2 : other.terms) {
					ret += std::pair<MultiIndex, double>{t1.first * t2.first, t1.second * t2.second};
				}
			}
			return ret;
		}

		////////////////////////////////////////////////////////
		/// Division
		////////////////////////////////////////////////////////

		[[nodiscard]] inline
		PolyDict operator/(const double v) const {
			if (std::abs(v) < YET_ANOTHER_TOL) {
				throw std::runtime_error{"Cannot divide by zero"};
			}
			PolyDict p{ideal};
			for (const auto& t: terms) {
				double c = t.second / v;
				if (std::abs(c) < YET_ANOTHER_TOL) {
					continue;
				}
				p.terms.emplace(t.first, c);
			}
			return p;
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const PolyDict& p) {
			if (p.terms.empty()) {
				return os << "0";
			}
			bool requires_plus = false;
			for (const auto& term: p.terms) {
				if (requires_plus) {
					os << " + ";
				} else {
					requires_plus = true;
				}

				const bool has_coeff = std::abs(term.second - 1) >= YET_ANOTHER_TOL;
				if (has_coeff) {
					os.precision(std::numeric_limits<double>::max_digits10 - 1);
					os << term.second;
				}
				if (term.first.is_constant()) {
					if (has_coeff) {
						continue;
					}
					os << 1;
					continue;
				}
				if (has_coeff) {
					os << " * ";
				}
				os << term.first;
			}
			return os;
		}
	};

	namespace simplify {
		[[nodiscard]] inline
		PolyDict constant(const Ideal& ideal, double value) {
			PolyDict p{ideal};
			p += std::pair<MultiIndex, double>{MultiIndex{ideal}, value};
			return p;
		}

		[[nodiscard]] inline
		std::optional<PolyDict> get_assignment_var(const PolyDict& poly, int index) {
			MultiIndex m{poly.ideal};
			m.set(index, 1);
			const auto& loc = poly.terms.find(m);
			if (loc == poly.terms.end()) {
				return {};
			}
			return poly.without(loc->first) / -loc->second;
		}

//		[[nodiscard]] inline
//		bool poly_is_linear_in(const Polynomial& poly, int index) {
//			Monomial m{1.0, poly.dim()};
//			for (int i=0; i<poly.dim(); i++) {
//				if (i == index) {
//					m.powers[index] = 1;
//				} else {
//					m.powers[index] = 500; // TODO
//				}
//			}
//			for (const Monomial& term: poly.terms) {
//				if (!term.divides(m)) {
//					return false;
//				}
//			}
//			return true;
//		}
	};


	namespace parse {
		// TODO: combine this...
		void parse_powers(std::string& str, MultiIndex& m) {
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
				m.set(index, power);
			}
		}

		std::optional<std::pair<MultiIndex, double>> parse_monomial(const Ideal& ideal, std::string& str) {
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

			std::pair<MultiIndex, double> m{ideal, coefficient.empty() ? 1.0 : std::stod(coefficient)};
			if (!powers.empty()) {
				parse_powers(powers, m.first);
			}
			return m;
		}

		PolyDict parse_polynomial(const Ideal& ideal, std::string& str) {
			PolyDict p{ideal};
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

				auto m = parse_monomial(ideal, term);
				if (!m) {
					continue;
				}
				if (sign == "-") {
					m->second *= -1;
				}
				p += *m;
			}
			return p;
		}
	}
}

#endif //IDEA_GMPPOLY_H
