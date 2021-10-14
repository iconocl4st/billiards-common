//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_INDEX_H
#define IDEA_INDEX_H

namespace algebra::poly {
#define POLY_TOL (1e-10)

	// template <class elem_type>
	class MultiIndex {
	public:
		const std::vector<int> powers;

		MultiIndex(const std::vector<int>& powers)
			: powers(powers)
		{}

		MultiIndex(int n)
			: powers{}
		{
			for (int i = 0; i < n; i++) {
				powers.push_back(0);
			}
		}

		[[nodiscard]] inline
		double evaluate(const std::vector<double>& x) const {
			if (dim() != x.size()) {
				throw std::runtime_error{"Can't evaluate at different dimension."};
			}
			double value = 1.0;
			for (int i = 0; i < dim(); i++) {
				if (powers[i] == 0) {
					continue;
				}
				value *= std::pow(x[i], powers[i]);
			}
			return value;
		}

		[[nodiscard]] inline
		int dim() const {
			return (int) powers.size();
		}

		[[nodiscard]] inline
		bool divides(const MultiIndex& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot check divisibility of multi index in different dimension"};
			}
			for (int i = 0; i < powers.size(); i++) {
				if (powers[i] > other.powers[i]) {
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] inline
		MultiIndex lcm(const MultiIndex& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot compute lcm in different dimensions"};
			}
			std::vector<int> maxes;
			for (int i = 0; i < (int) powers.size(); i++) {
				maxes.push_back(std::max(powers[i], other.powers[i]));
			}
			return MultiIndex{maxes};
		}

		[[nodiscard]] inline
		bool is_divisible_by(const MultiIndex& other) const {
			return other.divides(*this);
		}

		[[nodiscard]] inline
		int degree(int idx) const {
//			if (idx < 0 || idx >= powers.size()) {
//				throw std::runtime_error{};
//			}
			return powers[idx];
		}

		[[nodiscard]] inline
		int degree() const {
			int sum = 0;
			for (int power : powers) {
				sum += power;
			}
			return sum;
		}

		inline
		bool operator==(const Monomial& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot compare monomials in different dimension"};
			}
			for (int i = 0; i < powers.size(); i++) {
				if (powers[i] != other.powers[i]) {
					return false;
				}
			}
			if (std::abs(coefficient - other.coefficient) >= POLY_TOL) {
				return false;
			}
			return true;
		}
		inline
		bool operator<(const Monomial& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot compare monomials in different dimension"};
			}
			for (int i = 0; i < powers.size(); i++) {
				if (powers[i] < other.powers[i]) {
					return true;
				}
				if (powers[i] > other.powers[i]) {
					return false;
				}
			}
			if (coefficient < other.coefficient) {
				return true;
			}
			if (coefficient > other.coefficient) {
				return false;
			}
			return false;
		}

		inline bool operator<=(const Monomial& other) const {
			return operator==(other) || operator<(other);
		}
		inline bool operator>(const Monomial& other) const {
			return !(operator==(other)) && !(operator<(other));
		}
		inline bool operator>=(const Monomial& other) const {
			return operator==(other) || !(operator<(other));
		}

		inline
		std::ostream& write(std::ostream& os, const Formatter& f) const {
			const bool has_coeff = std::abs(coefficient - 1) > POLY_TOL;
			const bool deg_0 = degree() == 0;
			if (has_coeff) {
				os.precision(std::numeric_limits<double>::max_digits10 - 1);
				os << coefficient;
			}
			if (deg_0) {
				if (has_coeff) {
					return os;
				} else {
					return os << 1;
				}
			}
			bool requires_multiplication = has_coeff;
			for (int i = 0; i < powers.size(); i++) {
				if (powers[i] == 0) {
					continue;
				}
				if (requires_multiplication) {
					os << " * ";
				} else {
					requires_multiplication = true;
				}
				os << f(i);
				if (powers[i] != 1) {
					os << "^" << powers[i];
				}
			}
			return os;
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const Monomial& m) {
			auto def_formatter = default_formatter();
			return m.write(os, def_formatter);
		}

		[[nodiscard]] inline
		Monomial operator-() const {
			return Monomial{-coefficient, powers};
		}

		[[nodiscard]] inline
		Monomial operator*(const Monomial& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot add polynomials in different dimension"};
			}
			Monomial ret{coefficient * other.coefficient, dim()};
			for (int i = 0; i < powers.size(); i++) {
				ret.powers[i] = powers[i] + other.powers[i];
			}
			return ret;
		}

		[[nodiscard]] inline
		Monomial operator*(const double v) const {
			return Monomial{coefficient * v, powers};
		}

		[[nodiscard]] inline
		Monomial operator/(const double v) const {
			if (std::abs(v) < POLY_TOL) {
				throw std::runtime_error{"Division by zero"};
			}
			return Monomial{coefficient / v, powers};
		}

		[[nodiscard]] inline
		Monomial operator/(const Monomial& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot divide polynomials in different dimension"};
			}
			if (!is_divisible_by(other)) {
				throw std::runtime_error{"Not divisible"};
			}
			Monomial ret{coefficient / other.coefficient, dim()};
			for (int i = 0; i < powers.size(); i++) {
				ret.powers[i] = powers[i] - other.powers[i];
			}
			return ret;
		}

		[[nodiscard]] inline
		bool powers_match(const Monomial& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot compare monomials in different dimension"};
			}
			for (int i = 0; i < powers.size(); i++) {
				if (powers[i] != other.powers[i]) {
					return false;
				}
			}
			return true;
		}

		inline
		Monomial& operator+=(const Monomial& other) {
			if (!powers_match(other)) {
				throw std::runtime_error{"Cannot add monomials in different dimension"};
			}
			coefficient += other.coefficient;
			return *this;
		}

		[[nodiscard]] inline
		std::string powers_key() const {
			std::stringstream s;
			bool requires_comma = false;
			for (const int& p : powers) {
				if (requires_comma) {
					s << ",";
				} else {
					requires_comma = true;
				}
				s << p;
			}
			return s.str();
		}
	};
}
#endif //IDEA_INDEX_H
