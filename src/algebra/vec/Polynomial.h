////
//// Created by thallock on 10/19/21.
////
//
//#ifndef IDEA_POLYNOMIAL_H
//#define IDEA_POLYNOMIAL_H
//
//#include "algebra/Ideal.h"
//#include "algebra/Index.h"
//
//namespace algebra::poly {
//
//	typedef std::pair<std::shared_ptr<MultiIndex>, double> Monomial;
//
//	template<class C>
//	class Polynomial {
//	public:
//		Ideal& ideal;
//
//		explicit Polynomial(Ideal& ideal) : ideal{ideal} {}
//
//		virtual ~Polynomial() = default;
//
//		[[nodiscard]] virtual bool is_zero() const = 0;
//		virtual const C& terms() const = 0;
//		[[nodiscard]] virtual const Monomial& leading() const = 0;
//		[[nodiscard]] std::shared_ptr<Polynomial> copy() const = 0;
//
//		[[nodiscard]] inline
//		int dim() const {
//			return ideal.dim();
//		}
//
//		[[nodiscard]] inline
//		int degree() const {
//			int deg = -1;
//			for (const auto& term : terms()) {
//				deg = std::max(deg, term.first.degree());
//			}
//			return deg;
//		}
//
//		[[nodiscard]] inline
//		double evaluate(const std::vector<double>& x) const {
//			double value = 0;
//			for (const auto& term: *this) {
//				value += term.first->evaluate(x) * term.second;
//			}
//			return value;
//		}
//
//
//		[[nodiscard]] inline
//		std::shared_ptr<Polynomial<C>> to_monic() const {
//			return *this / leading()->second;
//		}
//
//
//		////////////////////////////////////////////////////////
//		/// Addition
//		////////////////////////////////////////////////////////
//
//		virtual void add(const Monomial& monomial) = 0;
//		virtual void add_new(const Monomial& monomial) = 0;
//	};
//
//
//
//	template <class C>
//	class PolyPtr {
//	public:
//		std::shared_ptr<Polynomial<C>> ptr;
//
//		explicit PolyPtr(std::shared_ptr<Polynomial<C>>& p)
//			: ptr{p}
//		{
//
//		}
//		virtual ~PolyPtr() = default;
//
//		////////////////////////////////////////////////////////
//		/// Addition
//		////////////////////////////////////////////////////////
//
//		void operator+=(const Monomial& monomial) {
//			ptr->add(monomial);
//		}
//
//		inline
//		PolyPtr operator+=(const PolyPtr& m) {
//			for (const auto& term: m->terms) {
//				operator+=(term);
//			}
//			return *this;
//		}
//
//		[[nodiscard]] inline
//		PolyPtr operator+(const PolyPtr& other) const {
//			auto p = ptr->copy();
//			p += other;
//			return p;
//		}
//
//		////////////////////////////////////////////////////////
//		/// Subtraction
//		////////////////////////////////////////////////////////
//
//		inline
//		void operator-=(const Monomial m) {
//			ptr->add(std::pair<std::shared_ptr<MultiIndex>, double>{m.first, -m.second});
//		}
//
//		inline
//		PolyPtr& operator-=(const PolyPtr& p) {
//			for (const auto& term: p.ptr->terms()) {
//				operator-=(term);
//			}
//			return *this;
//		}
//
//		[[nodiscard]] inline
//		PolyDict operator-(const PolyDict& other) const {
//			PolyDict p = copy();
//			for (const auto& t: other.terms) {
//				p -= t;
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyDict operator-(const double v) const {
//			PolyDict p = copy();
//			p -= std::pair<MultiIndex, double>{MultiIndex{ideal}, v};
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyDict operator-(const std::pair<MultiIndex, double>& other) const {
//			PolyDict p = copy();
//			p -= other;
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyDict operator-() const {
//			PolyDict p{ideal};
//			for (const auto& term: terms) {
//				p.terms.emplace(term.first, -term.second);
//			}
//			return p;
//		}
//
//		////////////////////////////////////////////////////////
//		/// Multiplication
//		////////////////////////////////////////////////////////
//
//		[[nodiscard]] inline
//		PolyDict operator*(const double v) const {
//			PolyDict p{ideal};
//			for (const auto& t: terms) {
//				const double c = t.second * v;
//				if (std::abs(c) < YET_ANOTHER_TOL) {
//					continue;
//				}
//				p.terms.emplace(t.first, c);
//			}
//			return p;
//		}
//
//		[[nodiscard]] inline
//		PolyDict operator*(const std::pair<MultiIndex, double>& pair) const {
//			PolyDict ret = PolyDict{ideal};
//			for (const auto& term: terms) {
//				double c = term.second * pair.second;
//				if (std::abs(c) < YET_ANOTHER_TOL) {
//					continue;
//				}
//				ret.terms.emplace(term.first * pair.first, c);
//			}
//			return ret;
//		}
//
//		[[nodiscard]] inline
//		PolyDict operator*(const MultiIndex& m) const {
//			return *this * std::pair<MultiIndex, double>{m, 1.0};
//		}
//
//		[[nodiscard]] inline
//		PolyDict operator*(const PolyDict& other) const {
//			PolyDict ret = PolyDict{ideal};
//			for (const auto& t1 : terms) {
//				for (const auto& t2 : other.terms) {
//					ret += std::pair<MultiIndex, double>{t1.first * t2.first, t1.second * t2.second};
//				}
//			}
//			return ret;
//		}
//
//		////////////////////////////////////////////////////////
//		/// Division
//		////////////////////////////////////////////////////////
//
//		[[nodiscard]] inline
//		PolyDict operator/(const double v) const {
//			if (std::abs(v) < YET_ANOTHER_TOL) {
//				throw std::runtime_error{"Cannot divide by zero"};
//			}
//			PolyDict p{ideal};
//			for (const auto& t: terms) {
//				double c = t.second / v;
//				if (std::abs(c) < YET_ANOTHER_TOL) {
//					continue;
//				}
//				p.terms.emplace(t.first, c);
//			}
//			return p;
//		}
//	};
//}
//
//#endif //IDEA_POLYNOMIAL_H
