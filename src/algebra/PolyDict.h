//
// Created by thallock on 10/13/21.
//

#ifndef IDEA_POLYDICT_H
#define IDEA_POLYDICT_H

#include <memory>
#include <utility>
#include <vector>
#include <map>
#include <regex>
#include <sstream>
#include <iostream>

#include "algebra/MultiIndexImpl.h"

namespace algebra::poly {

	typedef std::pair<std::shared_ptr<MultiIndex>, double> Monomial;

	class PolyDict {
	public:
		const std::shared_ptr<IndexImpl>& index_impl;
		std::map<
		    std::shared_ptr<MultiIndex>,
			double,
			std::function<bool(const IndexPtr&, const IndexPtr&)>> terms;

		explicit PolyDict(const std::shared_ptr<IndexImpl>& ii)
			: index_impl{ii}
			, terms{ii->get_comparator()}
		{

		}

		PolyDict(
			const std::shared_ptr<IndexImpl>& ii,
			std::map<
			    std::shared_ptr<MultiIndex>,
				double,
				std::function<bool(const IndexPtr&, const IndexPtr&)>> terms)
			: index_impl{ii}
			, terms{std::move(terms)}
		{
			if (!index_impl) {
				throw std::runtime_error{"Must have an index implementation"};
			}
		}

//		explicit PolyDict(const PolyDict& other)
//			: index_impl{other.index_impl}
//			, terms{other.terms}
//		{
//			if (!index_impl) {
//				throw std::runtime_error{"Must have an index implementation"};
//			}
//		}

		~PolyDict() = default;

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> empty() const {
			return std::make_shared<PolyDict>(index_impl);
		}

		[[nodiscard]] inline
		double evaluate(const std::vector<double>& vals) const {
			double ret = 0;
			for (const auto& term : terms) {
				ret += term.first->evaluate(vals) * term.second;
			}
			return ret;
		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> copy() const {
			return std::make_shared<PolyDict>(index_impl, terms);
		}

		void erase(const std::shared_ptr<MultiIndex>& index) {
			terms.erase(index);
		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> substitute(int index, const std::shared_ptr<PolyDict>& value) {
			// Could reduce the dimension...
			std::shared_ptr<PolyDict> p = empty();
			for (const auto& term: terms) {
				const int old_power = term.first->get(index);
				if (old_power == 0) {
					p->operator+=(term);
					continue;
				}
				auto rest = term.first->copy();
				rest->set(index, 0);
				p->operator+=(value->pow(old_power)->operator*(Monomial{rest, term.second}));
			}
			return p;
		}

		[[nodiscard]] inline
		Monomial leading() const {
			return *terms.rbegin();
		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> to_monic() const {
			return this->operator/(leading().second);
		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> without(const std::shared_ptr<MultiIndex>& term) const {
			std::shared_ptr<PolyDict> p = copy();
			p->erase(term);
			return p;
		}

		[[nodiscard]] inline
		bool is_zero() const {
			return terms.empty();
		}

		[[nodiscard]] inline
		bool is_zero(double tol) const {
			for (const auto& term : terms) {
				if (std::abs(term.second) >= tol) {
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> pow(int power) const {
			if (power == 0) {
				std::shared_ptr<PolyDict> p = empty();
				p->terms.emplace(index_impl->create_empty(), 1.0);
				return p;
			}
			std::shared_ptr<PolyDict> p = copy();
			for (int i = 1; i < power; i++) {
				p = this->operator*(p);
			}
			return p;
		}

		inline
		void set(const IndexPtr& index, const double value) {
			if (std::abs(value) < POLY_TOL) {
				terms.erase(index);
			} else {
//				terms.insert(index, value);
				terms[index] = value;
			}
		}

//		inline
//		PolyDict& operator=(const PolyDict& other) {
//			if (!index_impl) {
//				throw std::runtime_error{"Must have an index implementation"};
//			}
//			terms = other.terms;
//			return *this;
//		}


		////////////////////////////////////////////////////////
		/// Addition
		////////////////////////////////////////////////////////

		inline
		void operator+=(const Monomial& pair) {
			const auto& loc = terms.find(pair.first);
			if (loc == terms.end()) {
				set(pair.first, pair.second);
			} else {
				set(pair.first, loc->second + pair.second);
			}
		}

		inline
		PolyDict& operator+=(const std::shared_ptr<PolyDict>& m) {
			for (const auto& term: m->terms) {
				operator+=(term);
			}
			return *this;
		}

//		[[nodiscard]] inline
//		PolyDict operator+(const double v) const {
//			PolyDict p = copy();
//			p += std::pair<MultiIndex, double>{MultiIndex{ideal}, v};
//			return p;
//		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> operator+(const std::shared_ptr<PolyDict>& other) const {
			std::shared_ptr<PolyDict> p = copy();
			p->operator+=(other);
			return p;
		}

		////////////////////////////////////////////////////////
		/// Subtraction
		////////////////////////////////////////////////////////

		inline
		void operator-=(const Monomial& m) {
			const auto& loc = terms.find(m.first);
			if (loc == terms.end()) {
				set(m.first, -m.second);
			} else {
				set(m.first, loc->second - m.second);
			}
		}

		inline
		void operator-=(const std::shared_ptr<PolyDict>& m) {
			for (const auto& term: m->terms) {
				operator-=(term);
			}
		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> operator-(const std::shared_ptr<PolyDict>& other) const {
			std::shared_ptr<PolyDict> p = copy();
			for (const auto& t: other->terms) {
				p->operator-=(t);
			}
			return p;
		}

//		[[nodiscard]] inline
//		PolyDict operator-(const double v) const {
//			PolyDict p = copy();
//			p -= std::pair<MultiIndex, double>{MultiIndex{ideal}, v};
//			return p;
//		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> operator-(const Monomial& other) const {
			std::shared_ptr<PolyDict> p = copy();
			p->operator-=(other);
			return p;
		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> operator-() const {
			std::shared_ptr<PolyDict> p = empty();
			for (const auto& term: terms) {
				p->terms.emplace(term.first, -term.second);
			}
			return p;
		}

		////////////////////////////////////////////////////////
		/// Multiplication
		////////////////////////////////////////////////////////

//		[[nodiscard]] inline
//		PolyPtr operator*(const double v) const {
//			PolyPtr p {index_impl};
//			for (const auto& t: terms) {
//				const double c = t.second * v;
//				if (std::abs(c) < POLY_TOL) {
//					continue;
//				}
//				p.terms.emplace(t.first, c);
//			}
//			return p;
//		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> operator*(const Monomial& pair) const {
			std::shared_ptr<PolyDict> ret = empty();
			for (const auto& term: terms) {
				ret->operator+=(Monomial(term.first->mult(pair.first), term.second * pair.second));
			}
			return ret;
		}

//		[[nodiscard]] inline
//		PolyDict operator*(const Monomial& m) const {
//			return *this * std::pair<MultiIndex, double>{m, 1.0};
//		}

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> operator*(const std::shared_ptr<PolyDict>& other) const {
			std::shared_ptr<PolyDict> ret = empty();
			for (const auto& t1 : terms) {
				for (const auto& t2 : other->terms) {
					ret->operator+=(Monomial{t1.first->mult(t2.first), t1.second * t2.second});
				}
			}
			return ret;
		}

		////////////////////////////////////////////////////////
		/// Division
		////////////////////////////////////////////////////////

		[[nodiscard]] inline
		std::shared_ptr<PolyDict> operator/(const double v) const {
			if (std::abs(v) < POLY_TOL) {
				throw std::runtime_error{"Cannot divide by zero"};
			}
			std::shared_ptr<PolyDict> p = empty();
			for (const auto& t: terms) {
				double c = t.second / v;
				if (std::abs(c) < POLY_TOL) {
					continue;
				}
				p->terms.emplace(t.first, c);
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

				const bool has_coeff = std::abs(term.second - 1) >= POLY_TOL;
				if (has_coeff) {
					os.precision(std::numeric_limits<double>::max_digits10 - 1);
					os << term.second;
				}
				if (term.first->is_constant()) {
					if (has_coeff) {
						continue;
					}
					os << 1;
					continue;
				}
				if (has_coeff) {
					os << " * ";
				}
				os << *term.first;
			}
			return os;
		}
	};

	typedef std::shared_ptr<PolyDict> PolyPtr;

	namespace simplify {
		[[nodiscard]] inline
		PolyPtr constant(const std::shared_ptr<IndexImpl>& impl, double value) {
			PolyPtr p = std::make_shared<PolyDict>(impl);
			p->operator+=(Monomial(impl->create_empty(), value));
			return p;
		}

//		[[nodiscard]] inline
//		PolyPtr monomial(const IndexPtr& impl, double value) {
//			PolyPtr p = std::make_shared<PolyDict>(impl);
//			p->operator+=(Monomial(impl, value));
//			return p;
//		}

		[[nodiscard]] inline
		std::optional<PolyPtr> get_assignment_var(const PolyPtr& poly, int var_index) {
			auto index_ptr = poly->index_impl->create_empty();
			index_ptr->set(var_index, 1);
			const auto& loc = poly->terms.find(index_ptr);
			if (loc == poly->terms.end()) {
				return {};
			}
			for (const auto& term : poly->terms) {
				if (term.first->get(var_index) > 1) {
					return {};
				}
			}
			return poly->without(loc->first)->operator/(-loc->second);
		}

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
	};

	typedef std::shared_ptr<PolyDict> PolyPtr;




	[[nodiscard]] inline
	PolyPtr operator*(const PolyPtr& p1, const PolyPtr& p2) {
		return p1->operator*(p2);
	}
	[[nodiscard]] inline
	PolyPtr operator*(const PolyPtr& p1, const Monomial& p2) {
		return p1->operator*(p2);
	}
	[[nodiscard]] inline
	PolyPtr operator*(const PolyPtr& p1, const double v) {
		return p1->operator*(simplify::constant(p1->index_impl, v));
	}
	[[nodiscard]] inline
	PolyPtr operator/(const PolyPtr& p1, const double v) {
		if (std::abs(v) < POLY_TOL) {
			throw std::runtime_error{"division by zero"};
		}
		return p1 * (1.0 / v);
	}
	[[nodiscard]] inline
	PolyPtr operator-(const PolyPtr& p1, const PolyPtr& p2) {
		return p1->operator-(p2);
	}
	[[nodiscard]] inline
	PolyPtr operator+(const PolyPtr& p1, const PolyPtr& p2) {
		return p1->operator+(p2);
	}
	[[nodiscard]] inline
	PolyPtr operator-(const PolyPtr& p1) {
		return p1->operator-();
	}
	[[nodiscard]] inline
	PolyPtr operator-(const PolyPtr& p1, const double v) {
		return p1 - simplify::constant(p1->index_impl, v);
	}
	inline
	PolyPtr& operator+=(PolyPtr& p1, const PolyPtr& p2) {
		p1->operator+=(p2);
		return p1;
	}
	inline
	PolyPtr& operator-=(PolyPtr& p1, const PolyPtr& p2) {
		p1->operator-=(p2);
		return p1;
	}

	inline
	PolyPtr& operator+=(PolyPtr& p1, const Monomial& p2) {
		p1->operator+=(p2);
		return p1;
	}

	inline
	std::ostream& operator<<(std::ostream& os, const PolyPtr& p2) {
		return os << *p2;
	}
	inline
	std::ostream& operator<<(std::ostream& os, const Monomial& monomial) {
		return os << monomial.second << "*" << *monomial.first;
	}
}

#endif //IDEA_POLYDICT_H
