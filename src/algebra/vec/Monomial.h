////
//// Created by thallock on 10/11/21.
////
//
//#ifndef IDEA_MULTIINDEX_H
//#define IDEA_MULTIINDEX_H
//
//#include <utility>
//#include <vector>
//#include <iostream>
//
//#include "algebra/vec/VecIndex.h"
//
//namespace algebra::poly {
//#define POLY_TOL (1e-10)
//
//	class Monomial {
//	public:
//		double coefficient;
//		VectorIndex index;
//
//		Monomial(double coefficient, std::vector<int> powers)
//			: coefficient{coefficient}
//			, powers(std::move(powers))
//		{}
//
//		Monomial(double coefficient, int n)
//			: coefficient{coefficient}
//			, powers{}
//		{
//			for (int i = 0; i < n; i++) {
//				powers.push_back(0);
//			}
//		}
//
//		explicit Monomial(int n) : Monomial{0, n} {}
//
//
//		[[nodiscard]] inline
//		double evaluate(const std::vector<double>& x) const {
//			if (dim() != x.size()) {
//				throw std::runtime_error{"Can't evaluate at different dimension."};
//			}
//			double value = coefficient;
//			for (int i = 0; i < dim(); i++) {
//				if (powers[i] == 0) {
//					continue;
//				}
//				value *= std::pow(x[i], powers[i]);
//			}
//			return value;
//		}
//
//		[[nodiscard]] inline
//		int dim() const {
//			return (int) powers.size();
//		}
//
//		[[nodiscard]] inline
//		bool is_zero() const {
//			return std::abs(coefficient) < POLY_TOL;
//		}
//
//		[[nodiscard]] inline
//		bool divides(const Monomial& other) const {
//			if (dim() != other.dim()) {
//				throw std::runtime_error{"Cannot check divisibility of monomials in different dimension"};
//			}
//			if (is_zero()) {
//				return false;
//			}
//			for (int i = 0; i < powers.size(); i++) {
//				if (powers[i] > other.powers[i]) {
//					return false;
//				}
//			}
//			return true;
//		}
//
//		[[nodiscard]] inline
//		Monomial lcm(const Monomial& other) const {
//			if (dim() != other.dim()) {
//				throw std::runtime_error{"Cannot compute lcm in different dimensions"};
//			}
//			std::vector<int> maxes;
//			for (int i = 0; i < (int) powers.size(); i++) {
//				maxes.push_back(std::max(powers[i], other.powers[i]));
//			}
//			return Monomial{1.0, maxes};
//		}
//
//		[[nodiscard]] inline
//		bool is_divisible_by(const Monomial& other) const {
//			return other.divides(*this);
//		}
//
//		[[nodiscard]] inline
//		int degree(int idx) const {
////			if (idx < 0 || idx >= powers.size()) {
////				throw std::runtime_error{};
////			}
//			return powers[idx];
//		}
//
//		[[nodiscard]] inline
//		int degree() const {
//			int sum = 0;
//			for (int power : powers) {
//				sum += power;
//			}
//			return sum;
//		}
//
//		inline
//		bool operator==(const Monomial& other) const {
//			if (dim() != other.dim()) {
//				throw std::runtime_error{"Cannot compare monomials in different dimension"};
//			}
//			for (int i = 0; i < powers.size(); i++) {
//				if (powers[i] != other.powers[i]) {
//					return false;
//				}
//			}
//			if (std::abs(coefficient - other.coefficient) >= POLY_TOL) {
//				return false;
//			}
//			return true;
//		}
//		inline
//		bool operator<(const Monomial& other) const {
//			if (dim() != other.dim()) {
//				throw std::runtime_error{"Cannot compare monomials in different dimension"};
//			}
//			for (int i = 0; i < powers.size(); i++) {
//				if (powers[i] < other.powers[i]) {
//					return true;
//				}
//				if (powers[i] > other.powers[i]) {
//					return false;
//				}
//			}
//			if (coefficient < other.coefficient) {
//				return true;
//			}
//			if (coefficient > other.coefficient) {
//				return false;
//			}
//			return false;
//		}
//
//		inline bool operator<=(const Monomial& other) const {
//			return operator==(other) || operator<(other);
//		}
//		inline bool operator>(const Monomial& other) const {
//			return !(operator==(other)) && !(operator<(other));
//		}
//		inline bool operator>=(const Monomial& other) const {
//			return operator==(other) || !(operator<(other));
//		}
//
//		inline
//		std::ostream& write(std::ostream& os, const Formatter& f) const {
//			const bool has_coeff = std::abs(coefficient - 1) > POLY_TOL;
//			const bool deg_0 = degree() == 0;
//			if (has_coeff) {
//				os.precision(std::numeric_limits<double>::max_digits10 - 1);
//				os << coefficient;
//			}
//			if (deg_0) {
//				if (has_coeff) {
//					return os;
//				} else {
//					return os << 1;
//				}
//			}
//			bool requires_multiplication = has_coeff;
//			for (int i = 0; i < powers.size(); i++) {
//				if (powers[i] == 0) {
//					continue;
//				}
//				if (requires_multiplication) {
//					os << " * ";
//				} else {
//					requires_multiplication = true;
//				}
//				os << f(i);
//				if (powers[i] != 1) {
//					os << "^" << powers[i];
//				}
//			}
//			return os;
//		}
//
//		inline
//		friend std::ostream& operator<<(std::ostream& os, const Monomial& m) {
//			auto def_formatter = default_formatter();
//			return m.write(os, def_formatter);
//		}
//
//		[[nodiscard]] inline
//		Monomial operator-() const {
//			return Monomial{-coefficient, powers};
//		}
//
//		[[nodiscard]] inline
//		Monomial operator*(const Monomial& other) const {
//			if (dim() != other.dim()) {
//				throw std::runtime_error{"Cannot add polynomials in different dimension"};
//			}
//			Monomial ret{coefficient * other.coefficient, dim()};
//			for (int i = 0; i < powers.size(); i++) {
//				ret.powers[i] = powers[i] + other.powers[i];
//			}
//			return ret;
//		}
//
//		[[nodiscard]] inline
//		Monomial operator*(const double v) const {
//			return Monomial{coefficient * v, powers};
//		}
//
//		[[nodiscard]] inline
//		Monomial operator/(const double v) const {
//			if (std::abs(v) < POLY_TOL) {
//				throw std::runtime_error{"Division by zero"};
//			}
//			return Monomial{coefficient / v, powers};
//		}
//
//		[[nodiscard]] inline
//		Monomial operator/(const Monomial& other) const {
//			if (dim() != other.dim()) {
//				throw std::runtime_error{"Cannot divide polynomials in different dimension"};
//			}
//			if (!is_divisible_by(other)) {
//				throw std::runtime_error{"Not divisible"};
//			}
//			Monomial ret{coefficient / other.coefficient, dim()};
//			for (int i = 0; i < powers.size(); i++) {
//				ret.powers[i] = powers[i] - other.powers[i];
//			}
//			return ret;
//		}
//
//		[[nodiscard]] inline
//		bool powers_match(const Monomial& other) const {
//			if (dim() != other.dim()) {
//				throw std::runtime_error{"Cannot compare monomials in different dimension"};
//			}
//			for (int i = 0; i < powers.size(); i++) {
//				if (powers[i] != other.powers[i]) {
//					return false;
//				}
//			}
//			return true;
//		}
//
//		inline
//		Monomial& operator+=(const Monomial& other) {
//			if (!powers_match(other)) {
//				throw std::runtime_error{"Cannot add monomials in different dimension"};
//			}
//			coefficient += other.coefficient;
//			return *this;
//		}
//
//		[[nodiscard]] inline
//		std::string powers_key() const {
//			std::stringstream s;
//			bool requires_comma = false;
//			for (const int& p : powers) {
//				if (requires_comma) {
//					s << ",";
//				} else {
//					requires_comma = true;
//				}
//				s << p;
//			}
//			return s.str();
//		}
//	};
//}
//
//
//#endif //IDEA_MULTIINDEX_H
