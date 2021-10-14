//
// Created by thallock on 10/11/21.
//

#ifndef IDEA_POLYNOMIAL_H
#define IDEA_POLYNOMIAL_H

#include "algebra/Monomial.h"

namespace algebra::poly {

	class Polynomial {
	private:

	public:
		std::vector<Monomial> terms;
		int dimension;

		explicit Polynomial(int dim) : dimension{dim} {}

		[[nodiscard]] inline
		Polynomial substitute(int index, const Polynomial& value) {
			Polynomial p{dim()}; // Could reduce the dimension...
			for (const Monomial& monomial : terms) {
				const int old_power = monomial.powers[index];
				if (old_power == 0) {
					p += monomial;
					continue;
				}
				Monomial rest{monomial};
				rest.powers[index] = 0;
				p += value.pow(old_power) * rest;
			}
			return p;
		}

		[[nodiscard]] inline
		double evaluate(const std::vector<double>& x) const {
			double value = 0;
			for (const auto& term : terms) {
				value += term.evaluate(x);
			}
			return value;
		}

		[[nodiscard]] inline
		int dim() const {
			return dimension;
		}

		inline
		Polynomial& sort() {
			std::stable_sort(terms.begin(), terms.end());
			std::reverse(terms.begin(), terms.end());
			return *this;
		}

		[[nodiscard]] inline
		Polynomial nonzero() const {
			Polynomial p{dim()};
			for (const Monomial& term : terms) {
				if (term.is_zero()) {
					continue;
				}
				p += term;
			}
			return p;
		}

		[[nodiscard]] inline
		Polynomial to_monic() const {
			if (terms.empty()) {
				return Polynomial{dim()};
			}
			return copy() / terms[0].coefficient;
		}

		inline
		void erase_first() {
			terms.erase(terms.begin());
		}

		[[nodiscard]] inline
		Polynomial without(int term_index) const {
			Polynomial p{dim()};
			for (int i = 0; i < (int) terms.size(); i++) {
				if (i != term_index) {
					p += terms[i];
				}
			}
			return p;
		}

		[[nodiscard]] inline
		bool conan_is_zero() const {
			for (const auto& t : terms) {
				if (!t.is_zero()) {
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] inline
		bool is_zero() const {
			return collect().conan_is_zero();
		}

		[[nodiscard]] inline
		Polynomial canon() const {
//			std::cout << "\t\tCalling canon on: " << *this << std::endl;
//			std::cout << "\t\tComputed:" << collect().nonzero().sort().to_monic() << std::endl;
			return collect().nonzero().sort().to_monic();
		}

		[[nodiscard]] inline
		Polynomial copy() const {
			Polynomial p{dim()};
			for (const auto& t : terms) {
				p += t;
			}
			return p;
		}

		[[nodiscard]] inline
		Polynomial pow(int power) const {
			if (power == 0) {
				Polynomial p{dim()};
				p += Monomial{1.0, dim()};
				return p;
			}
			Polynomial p = copy();
			for (int i = 1; i < power; i++) {
				p = p * *this;
			}
			return p;
		}


		inline
		Polynomial& operator+=(const Monomial& m) {
			terms.push_back(m);
			return *this;
		}
		inline
		Polynomial& operator+=(const Polynomial& m) {
			for (const auto& t : m.terms) {
				operator+=(t);
			}
			return *this;
		}

		[[nodiscard]] inline
		Polynomial operator*(const Monomial& t2) const {
			Polynomial p{dim()};
			for (const auto& t1: terms) {
				p += t1 * t2;
			}
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator*(const Polynomial& other) const {
			Polynomial p{dim()};
			for (const auto& t1 : terms) {
				for (const auto& t2 : other.terms) {
					p += t1 * t2;
				}
			}
			return p;
		}

		inline
		Polynomial& operator-=(const Monomial& t) {
			operator+=(-t);
			return *this;
		}

		inline
		Polynomial& operator-=(const Polynomial& other) {
			for (const auto& t: other.terms) {
				operator-=(t);
			}
			return *this;
		}

		[[nodiscard]] inline
		Polynomial operator-(const Polynomial& other) const {
			Polynomial p = copy();
			for (const auto& t: other.terms) {
				p -= t;
			}
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator*(const double v) const {
			Polynomial p{dim()};
			for (const auto& t : terms) {
				p += t * v;
			}
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator/(const double v) const {
			Polynomial p{dim()};
			for (const auto& t : terms) {
				p += t / v;
			}
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator+(const Polynomial& other) const {
			Polynomial p = copy();
			for (const auto& t: other.terms) {
				p += t;
			}
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator+(const Monomial& other) const {
			Polynomial p = copy();
			p += other;
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator+(const double v) const {
			Polynomial p = copy();
			p += Monomial{v, dim()};
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator-(const Monomial& other) const {
			Polynomial p = copy();
			p -= other;
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator-(const double v) const {
			Polynomial p = copy();
			p -= Monomial{v, dim()};
			return p;
		}

		[[nodiscard]] inline
		Polynomial operator-() const {
			Polynomial p{dim()};
			for (const auto& t: terms) {
				p -= t;
			}
			return p;
		}

		inline
		std::ostream& write(std::ostream& os, const Formatter& f) const {
			if (terms.empty()) {
				return os << "0";
			}
			bool requires_plus = false;
			for (const auto& term : terms) {
				if (requires_plus) {
					os << " + ";
				} else {
					requires_plus = true;
				}
				term.write(os, f);
			}
			return os;
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
			auto def_formatter = default_formatter();
			return p.write(os, def_formatter);
		}

		[[nodiscard]] inline
		Polynomial collect() const {
			Polynomial ret{dim()};
			std::map<std::string, int> map;
			for (const Monomial& m : terms) {
				std::string key = m.powers_key();
				if (map.contains(key)) {
					ret.terms[map[key]] += m;
				} else {
					map[key] = (int) ret.terms.size();
					ret.terms.push_back(m);
				}
			}
			return ret;
		}
	};

	namespace simplify {
		[[nodiscard]] inline
		Polynomial constant(int dim, double value) {
			Polynomial p{dim};
			p += Monomial{value, dim};
			return p;
		}

		[[nodiscard]] inline
		int get_assignment_var(const Polynomial& poly, int index) {
			Monomial m{1.0, poly.dim()};
			m.powers[index] = 1;
			for (int i = 0; i < (int) poly.terms.size(); i++) {
				if (poly.terms[i].powers_match(m)) {
					return i;
				}
			}
			return -1;
		}

		[[nodiscard]] inline
		bool poly_is_linear_in(const Polynomial& poly, int index) {
			Monomial m{1.0, poly.dim()};
			for (int i=0; i<poly.dim(); i++) {
				if (i == index) {
					m.powers[index] = 1;
				} else {
					m.powers[index] = 500; // TODO
				}
			}
			for (const Monomial& term: poly.terms) {
				if (!term.divides(m)) {
					return false;
				}
			}
			return true;
		}
	}

	class PolynomialWithFormatter {
	public:
		const Polynomial& poly;
		const Formatter formatter;

		PolynomialWithFormatter(const Polynomial& p, const Formatter& f)
			: poly{p}
			, formatter{f}
		{}

		friend inline
		std::ostream& operator<<(std::ostream& os, const PolynomialWithFormatter& p) {
			return p.poly.write(os, p.formatter);
		}
	};
}


#endif //IDEA_POLYNOMIAL_H
