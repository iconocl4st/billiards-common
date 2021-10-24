//
// Created by thallock on 10/19/21.
//

#ifndef IDEA_IDEAL_H
#define IDEA_IDEAL_H


namespace algebra::poly {
#define POLY_TOL (1e-10)

	namespace cmp {
		enum order {
			GradedLexical,
			GradedReverseLexical,
			Lexical,
		};
	}

	class Ideal {
	public:
		const cmp::order order;
		std::vector<std::string> names;

		explicit Ideal(cmp::order order) : order{order}, names{} {}

		virtual ~Ideal() = default;

		[[nodiscard]] inline
		int dim() const {
			return (int) names.size();
		}

		friend std::ostream& operator<<(std::ostream& os, const Ideal& ideal) {
			os << "\t\tIdeal with " << ideal.names.size() << " variables:\n";
			bool requires_comma = false;
			for (const auto& name : ideal.names) {
				if (requires_comma) {
					os << ", ";
				} else {
					requires_comma = true;
				}
				os << name;
			}
			return os;
		}

		[[nodiscard]] inline
		const std::string& name(int index) const {
			return names[index];
		}

		[[nodiscard]] inline
		int get_var_index(const std::string& name) const {
			for (int i = 0; i < names.size(); i++) {
				if (names[i] == name) {
					return i;
				}
			}
			throw std::runtime_error{"variable not found"};
		}

		inline
		void register_var(const std::string& name) {
			for (const auto& prev: names) {
				if (name == prev) {
					return;
				}
			}
			names.push_back(name);
		}
	};
}


#endif //IDEA_IDEAL_H
