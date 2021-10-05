//
// Created by thallock on 10/3/21.
//

#ifndef IDEA_COMPLEX_H
#define IDEA_COMPLEX_H

namespace billiards::math {

	// TODO: Didn't realize c had its own complex type...

#define TOL 1e-15
#define LARGER_TOL 1e-7

	class Complex {
	public:
		double a;
		double b;

		Complex(double a, double b) : a{a}, b{b} {}

		Complex(double a) : a{a}, b{0} {}

#define FROM_EXP(r, theta) Complex{r * std::cos(theta), r * std::sin(theta)}

		[[nodiscard]] inline
		double r() const {
			return std::sqrt(a * a + b * b);
		}

		[[nodiscard]] inline
		double theta() const {
			return std::atan2(b, a);
		}

		[[nodiscard]] inline
		bool is_real() const {
			return std::abs(b) < LARGER_TOL;
		}

		[[nodiscard]] inline
		double real() const {
			return a;
		}

		[[nodiscard]] inline
		Complex operator*(const Complex& o) const {
			return Complex{a * o.a - b * o.b, a * o.b + b * o.a};
		}

		[[nodiscard]] inline
		Complex operator+(const Complex& o) const {
			return Complex{a + o.a, b + o.b};
		}

		[[nodiscard]] inline
		Complex operator-(const Complex& o) const {
			return Complex{a - o.a, b - o.b};
		}

		[[nodiscard]] inline
		Complex operator/(const Complex& o) const {
			return FROM_EXP(r() / o.r(), theta() - o.theta());
		}

		[[nodiscard]] inline
		Complex operator*(const double d) const {
			return Complex{a * d, b * d};
		}

		[[nodiscard]] inline
		Complex operator+(const double d) const {
			return Complex{a + d, b};
		}

		[[nodiscard]] inline
		Complex pow(const double exp) const {
			return FROM_EXP(std::pow(r(), exp), theta() * exp);
		}

		friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
			return os << c.a << " + " << c.b << "i";
		}
	};
}
#endif //IDEA_COMPLEX_H
