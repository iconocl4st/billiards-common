//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_MULTIINDEXIMPL_H
#define IDEA_MULTIINDEXIMPL_H

#include <utility>

#include "algebra/GmpIndex.h"
#include "algebra/VecIndex.h"

namespace algebra::poly {

	typedef std::function<bool(const IndexPtr&, const IndexPtr&)> MonomialOrder;

	class IndexImpl {
	public:
		std::shared_ptr<Ideal> ideal;

		explicit IndexImpl(std::shared_ptr<Ideal> ideal) : ideal{std::move(ideal)} {}
		virtual ~IndexImpl() = default;

		[[nodiscard]] virtual std::shared_ptr<MultiIndex> create_empty() const = 0;
		[[nodiscard]] virtual std::function<bool(const IndexPtr&, const IndexPtr&)> get_comparator() const = 0;
	};

	class VectorIndexImpl : public IndexImpl {
	public:
		explicit VectorIndexImpl(const std::shared_ptr<Ideal>& ideal) : IndexImpl{ideal} {}
		~VectorIndexImpl() override = default;

		[[nodiscard]] std::shared_ptr<MultiIndex> create_empty() const override {
			auto ret = std::make_shared<VectorIndex>(ideal);
			return std::dynamic_pointer_cast<MultiIndex>(ret);
		};

		[[nodiscard]] inline
		MonomialOrder get_comparator() const override {
			switch (ideal->order) {
				case cmp::Lexical:
					return lex_less;
				case cmp::GradedLexical:
					return graded_lex_less;
				case cmp::GradedReverseLexical:
					return graded_reverse_lex_less;
				default:
					throw std::runtime_error{"Unknown comparison"};
			}
		}
	};

	class GmpIndexImpl : public IndexImpl {
	public:
		explicit GmpIndexImpl(const std::shared_ptr<Ideal>& ideal) : IndexImpl{ideal} {}
		~GmpIndexImpl() override = default;

		[[nodiscard]]
		std::shared_ptr<MultiIndex> create_empty() const override {
			auto ret = std::make_shared<GmpIndex>(ideal);
			return std::dynamic_pointer_cast<MultiIndex>(ret);
		};

		[[nodiscard]] MonomialOrder get_comparator() const override {
			switch (ideal->order) {
				case cmp::Lexical:
					return gmp_lex_less;
				case cmp::GradedLexical:
					return graded_lex_less;
				case cmp::GradedReverseLexical:
					return graded_reverse_lex_less;
				default:
					throw std::runtime_error{"Unknown comparison"};
			}
		}
	};
}
#endif //IDEA_MULTIINDEXIMPL_H
