//
// Created by thallock on 10/4/21.
//

#ifndef IDEA_NEWTON_H
#define IDEA_NEWTON_H


namespace billiards::math {

	[[nodiscard]] inline
	double newton(
		const std::function<double(const double)>& f,
		const std::function<double(const double)>& g,
		const double tol,
		const double x0
	) {
		double x = x0;
		for (int i = 0; i < 30; i++) {
			// f(x) + g(x) * (t - x) == 0
			// t = x - f(x) / g(x)
			const double fx = f(x);
			const double gx = g(x);
			if (std::abs(gx) < 1e-8) {
				break;
			}
			x += fx / gx;
		}
		if (std::abs(f(x)) < std::abs(f(x0))) {
			return x;
		} else {
			return x0;
		}
	}
	/*
t, x, h, g, f = var('t x h g f')
sols = solve(f + g * (t - x) + h * (t - x)^2 == 0, t)
sols[0].right()
sols[1].right()

	 */
	[[nodiscard]] inline
	double newton2(
		const std::function<double(const double)>& f,
		const std::function<double(const double)>& g,
		const std::function<double(const double)>& h,
		const double tol,
		const double x0
	) {
		double x = x0;
		for (int i = 0; i < 30; i++) {
			// f(x) + g(x) * (t - x) + h(x) * (t - x)^2 == 0
			// 1/2*(2*h*x - g - sqrt(g^2 - 4*f*h))/h
			// 1/2*(2*h*x - g + sqrt(g^2 - 4*f*h))/h

			// t = x - f(x) / g(x)
			const double fx = f(x);
			const double gx = g(x);
			const double hx = h(x);
			if (std::abs(hx) < 1e-8 || gx * x - 4 * fx * hx < 0) {
				break;
			}
			const double x1 = (2 * hx * x - gx - std::sqrt(gx * x - 4 * fx * hx)) / (2 * hx);
			const double x2 = (2 * hx * x - gx - std::sqrt(gx * x - 4 * fx * hx)) / (2 * hx);
			if (std::abs(f(x1)) < std::abs(f(x2))) {
				x = x1;
			} else {
				x = x2;
			}
		}
		if (std::abs(f(x)) < std::abs(f(x0))) {
			return x;
		} else {
			return x0;
		}
	}

	[[nodiscard]] inline
	double sharpen_quadratic(const double c0, const double c1, const double c2, const double x0) {
		return newton(
			[c0, c1, c2](const double x) { return c0 + c1 * x + c2 * x * x; },
			[c1, c2](const double x) { return c1 + 2 * c2 * x; },
			1e-16,
			x0
		);
	}
	[[nodiscard]] inline
	double sharpen_cubic(const double c0, const double c1, const double c2, const double c3, const double x0) {
		return newton2(
			[c0, c1, c2, c3](const double x) { return c0 + c1 * x + c2 * x * x + c3 * x * x * x; },
			[c1, c2, c3](const double x) { return c1 + 2 * c2 * x + 3 * c3 * x * x; },
			[c2, c3](const double x) { return 2 * c2 + 6 * c3 * x; },
			1e-16,
			x0
		);
	}
	[[nodiscard]] inline
	double sharpen_quartic(const double c0, const double c1, const double c2, const double c3, const double c4, const double x0) {
		return newton2(
			[c0, c1, c2, c3, c4](const double x) { return c0 + c1 * x + c2 * x * x + c3 * x * x * x + c4 * x * x * x * x; },
			[c1, c2, c3, c4](const double x) { return c1 + 2 * c2 * x + 3 * c3 * x * x + 4 * c4 * x * x * x; },
			[c2, c3, c4](const double x) { return 2 * c2 + 6 * c3 * x + 12 * c4 * x * x; },
			1e-16,
			x0
		);
	}
}

#endif //IDEA_NEWTON_H
