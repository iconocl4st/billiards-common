//
// Created by thallock on 10/11/21.
//

#ifndef IDEA_POLYVEC_H
#define IDEA_POLYVEC_H

#include "Monomial.h"

#include "Polynomial.h"

namespace algebra::poly {

	class PolyVec : Polynomial {
	public:
		std::vector<Monomial> terms;

		explicit PolyVec(Ideal& ideal) : Polynomial(ideal) {}

//		[[nodiscard]] inline
//		PolyVec substitute(int index, const PolyVec& value) {
//			PolyVec p{dim()}; // Could reduce the dimension...
//			for (const Monomial& monomial : terms) {
//				const int old_power = monomial.powers[index];
//				if (old_power == 0) {
//					p += monomial;
//					continue;
//				}
//				Monomial rest{monomial};
//				rest.powers[index] = 0;
//				p += value.pow(old_power) * rest;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		double evaluate(const std::vector<double>& x) const {
//			double value = 0;
//			for (const auto& term : terms) {
//				value += term.evaluate(x);
//			}
//			return value;
//		}
//
//		[[nodiscard]] inline
//		int dim() const {
//			return dimension;
//		}
//
//		inline
//		PolyVec& sort() {
//			std::stable_sort(terms.begin(), terms.end());
//			std::reverse(terms.begin(), terms.end());
//			return *this;
//		}
//
//		[[nodiscard]] inline
//		PolyVec nonzero() const {
//			PolyVec p{dim()};
//			for (const Monomial& term : terms) {
//				if (term.is_zero()) {
//					continue;
//				}
//				p += term;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec to_monic() const {
//			if (terms.empty()) {
//				return PolyVec{dim()};
//			}
//			return copy() / terms[0].coefficient;
//		}
//
//		inline
//		void erase_first() {
//			terms.erase(terms.begin());
//		}
//
//		[[nodiscard]] inline
//		PolyVec without(int term_index) const {
//			PolyVec p{dim()};
//			for (int i = 0; i < (int) terms.size(); i++) {
//				if (i != term_index) {
//					p += terms[i];
//				}
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		bool conan_is_zero() const {
//			for (const auto& t : terms) {
//				if (!t.is_zero()) {
//					return false;
//				}
//			}
//			return true;
//		}
//
//		[[nodiscard]] inline
//		bool is_zero() const {
//			return collect().conan_is_zero();
//		}
//
//		[[nodiscard]] inline
//		PolyVec canon() const {
////			std::cout << "\t\tCalling canon on: " << *this << std::endl;
////			std::cout << "\t\tComputed:" << collect().nonzero().sort().to_monic() << std::endl;
//			return collect().nonzero().sort().to_monic();
//		}
//
//		[[nodiscard]] inline
//		PolyVec copy() const {
//			PolyVec p{dim()};
//			for (const auto& t : terms) {
//				p += t;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec pow(int power) const {
//			if (power == 0) {
//				PolyVec p{dim()};
//				p += Monomial{1.0, dim()};
//				return p;
//			}
//			PolyVec p = copy();
//			for (int i = 1; i < power; i++) {
//				p = p * *this;
//			}
//			return p;
//		}
//
//
//		inline
//		PolyVec& operator+=(const Monomial& m) {
//			terms.push_back(m);
//			return *this;
//		}
//		inline
//		PolyVec& operator+=(const PolyVec& m) {
//			for (const auto& t : m.terms) {
//				operator+=(t);
//			}
//			return *this;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator*(const Monomial& t2) const {
//			PolyVec p{dim()};
//			for (const auto& t1: terms) {
//				p += t1 * t2;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator*(const PolyVec& other) const {
//			PolyVec p{dim()};
//			for (const auto& t1 : terms) {
//				for (const auto& t2 : other.terms) {
//					p += t1 * t2;
//				}
//			}
//			return p;
//		}
//
//		inline
//		PolyVec& operator-=(const Monomial& t) {
//			operator+=(-t);
//			return *this;
//		}
//
//		inline
//		PolyVec& operator-=(const PolyVec& other) {
//			for (const auto& t: other.terms) {
//				operator-=(t);
//			}
//			return *this;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator-(const PolyVec& other) const {
//			PolyVec p = copy();
//			for (const auto& t: other.terms) {
//				p -= t;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator*(const double v) const {
//			PolyVec p{dim()};
//			for (const auto& t : terms) {
//				p += t * v;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator/(const double v) const {
//			PolyVec p{dim()};
//			for (const auto& t : terms) {
//				p += t / v;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator+(const PolyVec& other) const {
//			PolyVec p = copy();
//			for (const auto& t: other.terms) {
//				p += t;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator+(const Monomial& other) const {
//			PolyVec p = copy();
//			p += other;
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator+(const double v) const {
//			PolyVec p = copy();
//			p += Monomial{v, dim()};
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator-(const Monomial& other) const {
//			PolyVec p = copy();
//			p -= other;
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator-(const double v) const {
//			PolyVec p = copy();
//			p -= Monomial{v, dim()};
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyVec operator-() const {
//			PolyVec p{dim()};
//			for (const auto& t: terms) {
//				p -= t;
//			}
//			return p;
//		}
//
//		inline
//		std::ostream& write(std::ostream& os, const Formatter& f) const {
//			if (terms.empty()) {
//				return os << "0";
//			}
//			bool requires_plus = false;
//			for (const auto& term : terms) {
//				if (requires_plus) {
//					os << " + ";
//				} else {
//					requires_plus = true;
//				}
//				term.write(os, f);
//			}
//			return os;
//		}
//
//		inline
//		friend std::ostream& operator<<(std::ostream& os, const PolyVec& p) {
//			auto def_formatter = default_formatter();
//			return p.write(os, def_formatter);
//		}
//
//		[[nodiscard]] inline
//		PolyVec collect() const {
//			PolyVec ret{dim()};
//			std::map<std::string, int> map;
//			for (const Monomial& m : terms) {
//				std::string key = m.powers_key();
//				if (map.contains(key)) {
//					ret.terms[map[key]] += m;
//				} else {
//					map[key] = (int) ret.terms.size();
//					ret.terms.push_back(m);
//				}
//			}
//			return ret;
//		}
	};

//	namespace simplify {
//		[[nodiscard]] inline
//		PolyVec constant(int dim, double value) {
//			PolyVec p{dim};
//			p += Monomial{value, dim};
//			return p;
//		}
//
//		[[nodiscard]] inline
//		int get_assignment_var(const PolyVec& poly, int index) {
//			Monomial m{1.0, poly.dim()};
//			m.powers[index] = 1;
//			for (int i = 0; i < (int) poly.terms.size(); i++) {
//				if (poly.terms[i].powers_match(m)) {
//					return i;
//				}
//			}
//			return -1;
//		}
//
//		[[nodiscard]] inline
//		bool poly_is_linear_in(const PolyVec& poly, int index) {
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
//	}
//
//	class PolynomialWithFormatter {
//	public:
//		const PolyVec& poly;
//		const Formatter formatter;
//
//		PolynomialWithFormatter(const PolyVec& p, const Formatter& f)
//			: poly{p}
//			, formatter{f}
//		{}
//
//		friend inline
//		std::ostream& operator<<(std::ostream& os, const PolynomialWithFormatter& p) {
//			return p.poly.write(os, p.formatter);
//		}
//	};
}


#endif //IDEA_POLYVEC_H
