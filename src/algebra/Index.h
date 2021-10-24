//
// Created by thallock on 10/12/21.
//

#ifndef IDEA_INDEX_H
#define IDEA_INDEX_H

#include "algebra/Ideal.h"

#include <memory>
#include <utility>

namespace algebra::poly {

	class MultiIndex {
	public:
		const std::shared_ptr<Ideal>& ideal;

		explicit MultiIndex(const std::shared_ptr<Ideal>& ideal) : ideal{ideal} {};

		virtual ~MultiIndex() = default;

		[[nodiscard]] virtual int get(int var_index) const = 0;

		virtual void set(int var_index, int power) = 0;

		[[nodiscard]] virtual std::shared_ptr<MultiIndex> copy() const = 0;

		[[nodiscard]] virtual std::shared_ptr<MultiIndex> empty() const = 0;

		[[nodiscard]] virtual bool is_constant() const { return degree() == 0; }

		inline
		virtual bool operator==(const MultiIndex& other) const {
			if (dim() != other.dim()) {
				throw std::runtime_error{"Cannot compare monomials in different dimension"};
			}
			for (int i = 0; i < dim(); i++) {
				if (get(i) != other.get(i)) {
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] inline
		virtual std::shared_ptr<MultiIndex> mult(const std::shared_ptr<MultiIndex>& other) const {
			if (dim() != other->dim()) {
				throw std::runtime_error{"Cannot add indices in different dimension"};
			}
			auto ret = empty();
			for (int i = 0; i < dim(); i++) {
				ret->set(i, get(i) + other->get(i));
			}
			return ret;
		}

		[[nodiscard]] inline
		virtual std::shared_ptr<MultiIndex> div(const std::shared_ptr<MultiIndex>& other) const {
			if (dim() != other->dim()) {
				throw std::runtime_error{"Cannot divide indices in different dimension"};
			}
			if (!is_divisible_by(other)) {
				throw std::runtime_error{"Not divisible"};
			}
			auto ret = empty();
			for (int i = 0; i < dim(); i++) {
				ret->set(i, get(i) - other->get(i));
			}
			return ret;
		}

		[[nodiscard]] inline
		double evaluate(const std::vector<double>& x) const {
			if (dim() != x.size()) {
				throw std::runtime_error{"Can't evaluate at different dimension."};
			}
			double value = 1.0;
			for (int i = 0; i < dim(); i++) {
				int power = get(i);
				if (power == 0) {
					continue;
				}
				value *= std::pow(x[i], power);
			}
			return value;
		}

		[[nodiscard]] inline
		int dim() const {
			return (int) ideal->dim();
		}

		[[nodiscard]] inline
		bool divides(const std::shared_ptr<MultiIndex>& other) const {
			if (dim() != other->dim()) {
				throw std::runtime_error{"Cannot check divisibility of multi index in different dimension"};
			}
			for (int i = 0; i < dim(); i++) {
				if (get(i) > other->get(i)) {
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] inline
		bool is_divisible_by(const std::shared_ptr<MultiIndex>& other) const {
			if (dim() != other->dim()) {
				throw std::runtime_error{"Cannot check divisibility of multi index in different dimension"};
			}
			for (int i = 0; i < dim(); i++) {
				if (get(i) < other->get(i)) {
					return false;
				}
			}
			return true;
		}

		[[nodiscard]] inline
		std::shared_ptr<MultiIndex> lcm(const std::shared_ptr<MultiIndex>& other) const {
			if (dim() != other->dim()) {
				throw std::runtime_error{"Cannot compute lcm in different dimensions"};
			}
			auto maxes = empty();
			for (int i = 0; i < dim(); i++) {
				maxes->set(i, std::max(get(i), other->get(i)));
			}
			return maxes;
		}

		[[nodiscard]] inline
		int degree() const {
			int sum = 0;
			for (int i = 0; i < dim(); i++) {
				sum += get(i);
			}
			return sum;
		}

		inline
		friend std::ostream& operator<<(std::ostream& os, const MultiIndex& m) {
			if (m.is_constant()) {
				return os << "1";
			}

			bool requires_multiplication = false;
			for (int i = 0; i < m.ideal->dim(); i++) {
				const int p = m.get(i);
				if (p == 0) {
					continue;
				}
				if (requires_multiplication) {
					os << "*";
				} else {
					requires_multiplication = true;
				}
				os << m.ideal->name(i);
				if (p != 1) {
					os << "^" << p;
				}
			}
			return os;
		}

//		[[nodiscard]] inline
//		std::string powers_key() const {
//			std::stringstream s;
//			bool requires_comma = false;
//			for (int i = 0; i < dim(); i++) {
//				if (requires_comma) {
//					s << ",";
//				} else {
//					requires_comma = true;
//				}
//				s << get(i);
//			}
//			return s.str();
//		}
	};


	typedef std::shared_ptr<MultiIndex> IndexPtr;

	[[nodiscard]] inline
	bool lex_less(const IndexPtr& idx1, const IndexPtr& idx2) {
		if (idx1->dim() != idx2->dim()) {
			throw std::runtime_error{"Cannot compare monomials in different dimension"};
		}
		for (int i = 0; i < idx1->dim(); i++) {
			int p1 = idx1->get(i);
			int p2 = idx2->get(i);
			if (p1 < p2) {
				return true;
			}
			if (p1 > p2) {
				return false;
			}
		}
		return false;
	}

	[[nodiscard]] inline
	bool graded_lex_less(const IndexPtr& idx1, const IndexPtr& idx2) {
		if (idx1->dim() != idx2->dim()) {
			throw std::runtime_error{"Cannot compare monomials in different dimension"};
		}
		int total_degree1 = idx1->degree();
		int total_degree2 = idx2->degree();
		if (total_degree1 < total_degree2) {
			return true;
		}
		if (total_degree1 > total_degree2) {
			return false;
		}
		return lex_less(idx1, idx2);
	}

	[[nodiscard]] inline
	bool graded_reverse_lex_less(const IndexPtr& idx1, const IndexPtr& idx2) {
		if (idx1->dim() != idx2->dim()) {
			throw std::runtime_error{"Cannot compare monomials in different dimension"};
		}
		int total_degree1 = idx1->degree();
		int total_degree2 = idx2->degree();
		if (total_degree1 < total_degree2) {
			return true;
		}
		if (total_degree1 > total_degree2) {
			return false;
		}
		for (int i = idx1->dim() - 1; i >= 0; i--) {
			int p1 = idx1->get(i);
			int p2 = idx2->get(i);
			if (p1 < p2) {
				return false;
			}
			if (p1 > p2) {
				return true;
			}
		}
		return false;
	}
}
#endif //IDEA_INDEX_H
