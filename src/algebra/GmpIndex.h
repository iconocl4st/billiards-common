//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_GMPINDEX_H
#define IDEA_GMPINDEX_H

#include <gmpxx.h>
#include <gmp.h>

#include "algebra/Index.h"
#include "GmpIdeal.h"

namespace algebra::poly {

	class GmpIndex : public MultiIndex {
	public:
		mpz_t powers;

		GmpIndex(const std::shared_ptr<Ideal>& ideal, const mpz_t& p)
			: MultiIndex{ideal}
			, powers{}
		{
			mpz_init_set(powers, p);
		}

		explicit GmpIndex(const std::shared_ptr<Ideal>& ideal)
			: MultiIndex{ideal},
			powers{}
		{
			mpz_init2(powers, ideal->dim() * get_log_2_max_deg());
		}

		~GmpIndex() override {
			mpz_clear(powers);
		}

		[[nodiscard]] inline
		int get_log_2_max_deg() const {
			const auto& gmp_ideal = std::dynamic_pointer_cast<GmpIdeal>(ideal);
			return gmp_ideal->log_2_max_deg;
		}

		inline
		void set(int var, int power) override {
			mpz_t term;
			mpz_init_set_ui(term, power);
			mpz_mul_2exp(term, term, var * get_log_2_max_deg());
			mpz_add(powers, powers, term);
		}

		[[nodiscard]] inline
		int get(int var) const override {
			mpz_t ret;
			mpz_init_set(ret, powers);
			mpz_div_2exp(ret, ret, var * get_log_2_max_deg());
			mpz_mod_2exp(ret, ret, get_log_2_max_deg());
			return mpz_get_ui(ret);
		}

		[[nodiscard]] inline
		std::shared_ptr<MultiIndex> copy() const override {
			auto ret = std::make_shared<GmpIndex>(ideal, powers);
			return std::dynamic_pointer_cast<MultiIndex>(ret);
		}

		[[nodiscard]] inline
		std::shared_ptr<MultiIndex> empty() const override {
			auto ret = std::make_shared<GmpIndex>(ideal);
			return std::dynamic_pointer_cast<MultiIndex>(ret);
		}

		[[nodiscard]] inline
		bool is_constant() const override {
			return mpz_sgn(powers) == 0;
		}

		inline
		GmpIndex& operator=(const GmpIndex& other) {
			mpz_set(powers, other.powers);
			return *this;
		}

		[[nodiscard]] inline
		bool operator==(const GmpIndex& other) const {
			return mpz_cmp(powers, other.powers) == 0;
		}

		[[nodiscard]] inline
		std::shared_ptr<MultiIndex> mult(const IndexPtr& other) const override {
			auto gmp_other = std::dynamic_pointer_cast<GmpIndex>(other);
			auto ret = empty();
			auto r = std::dynamic_pointer_cast<GmpIndex>(ret);
			mpz_add(r->powers, powers, gmp_other->powers);
			return ret;
		}

		[[nodiscard]] inline
		std::shared_ptr<MultiIndex> div(const IndexPtr& other) const override {
			auto gmp_other = std::dynamic_pointer_cast<GmpIndex>(other);
			auto ret = empty();
			auto r = std::dynamic_pointer_cast<GmpIndex>(ret);
			mpz_sub(r->powers, powers, gmp_other->powers);
			return ret;
		}
	};

	[[nodiscard]] inline
	bool gmp_lex_less(const IndexPtr& idx1, const IndexPtr& idx2) {
		const auto& gmp_idx1 = std::dynamic_pointer_cast<GmpIndex>(idx1);
		const auto& gmp_idx2 = std::dynamic_pointer_cast<GmpIndex>(idx2);
		return mpz_cmp(gmp_idx1->powers, gmp_idx2->powers) < 0;
	}
}

#endif //IDEA_GMPINDEX_H
