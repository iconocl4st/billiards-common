//
// Created by thallock on 10/2/21.
//

#ifndef IDEA_ORIENT_POCKET_H
#define IDEA_ORIENT_POCKET_H

#include <list>

#include "billiards_common/geometry/Maybe.h"
#include "billiards_common/math/polynomial.h"

//extern "C" {
//#include "math/orient_pocket_gen/gen_sol_0.h"
//#include "math/orient_pocket_gen/gen_sol_1.h"
//#include "math/orient_pocket_gen/gen_sol_2.h"
//#include "math/orient_pocket_gen/gen_sol_3.h"
//};


namespace billiards::shots::math {
	/*
	 *
	 * u = pocket segment 1
	 * v = pocket segment 2
	 * p = point of interest
	 * a = cue position
	 *
	 * Find the point
	 * 	p(t) = {
	 * 		(1 - t) * u.x + t * v.x
	 * 		(1 - t) * u.y + t * v.y
	 * 	}
	 *
	 * 	such that for all s, the point
	 * 	w(s, t) = {
	 * 		(1 - s) * a.x + s * p.x,
	 * 		(1 - s) * a.y + s * p.y,
	 * 	}
	 * 	has
	 * 	d(s, t) = |w - u| ^ 2 >= r ^ 2.
	 *
	 *
	 * 	First, we shift so that u is at the origin:
	 * 	a <-- a - u, v <-- v - u, p <-- p - u, u <-- 0
	 *
	 * 	p(t)	= { t * v.x,					, t * v.y						}
	 * 	w(s, t)	= { (1 - s) * a.x + s * t * v.x	, (1 - s) * a.y + s * t * v.y	}
	 *
	 * 	d(s, t) = |w - 0| ^ 2
	 * 			= ((1 - s) * a.x + s * t * v.x)^2
	 * 				+ ((1 - s) * a.y + s * t * v.y)^2
	 * 			= (1 - s)^2 * a.x^2 + 2 * (1 - s) * a.x * s * t * v.x + s^2 * t^2 * v.x^2
	 * 				+ (1 - s)^2 * a.y^2 + 2 * (1 - s) * a.y * s * t * v.y + s^2 * t^2 * v.y^2
	 * 			= (1 - s)^2 * (a.x^2 + a.y^2) + 2 * (1 - s) * s * t * (a.x * v.x + a.y * v.y)
	 * 				+ s^2 * t^2 * (v.x^2 + v.y^2)
	 *
	 * 	Let
	 * 		a2 = a.x^2 + a.y^2,
	 * 		av = a.x * v.x + a.y * v.y,
	 * 		v2 = v.x^2 + v.y^2
	 * 	So
	 * 		d(s, t) = (1 - s)^2 * a2 + 2 * (1 - s) * s * t * av + s^2 * t^2 * v2
	 *
	 *
a2, av, v2, s, t, r = var('a2 av v2 s t r')
d = (1 - s)^2 * a2 + 2 * (1 - s) * s * t * av + s^2 * t^2 * v2
expand(d).collect(s)
	 * 				= (t^2*v2 - 2*av*t + a2)*s^2 + 2*(av*t - a2)*s + a2
	 *
	 * 		d/ds[d(s, t)] = 2 * (t^2*v2 - 2*av*t + a2)*s + 2*(av*t - a2) = 0
	 * 		=>		s = -(av*t - a2) / (t^2*v2 - 2*av*t + a2)
	 * 				if t^2*v2 - 2*av*t + a2 =/= 0
	 *
	 * 		Want:
	 * 		d(t) == r^2
	 * 		=>	(t^2*v2 - 2*av*t + a2)*s^2 + 2*(av*t - a2)*s + a2 - r^2 == 0
	 * 		=>	(t^2*v2 - 2*av*t + a2)*(av*t - a2)^2
	 * 				- 2*(av*t - a2) * (av*t - a2) * (t^2*v2 - 2*av*t + a2)
	 * 				+ (a2 - r^2) * (t^2*v2 - 2*av*t + a2)^2 == 0
a2, av, v2, t, r = var('a2 av v2 t r')
expand(
	(t^2*v2 - 2*av*t + a2)*(av*t - a2)^2
		- 2*(av*t - a2) * (av*t - a2) * (t^2*v2 - 2*av*t + a2)
		+ (a2 - r^2) * (t^2*v2 - 2*av*t + a2)^2
).collect(t)

	 * 		=>
	 * 			- (r^2*v2^2 + av^2*v2 - a2*v2^2)*t^4
	 * 			+ 2*(2*av*r^2*v2 + av^3 - a2*av*v2)*t^3
	 * 			- (4*av^2*r^2 + 2*a2*r^2*v2 + a2*av^2 - a2^2*v2)*t^2
	 * 			+ 4*a2*av*r^2*t
	 * 			- a2^2*r^2  == 0
Sage:

export REPOS="/mnt/1f0ab4b3-c472-49e1-92d8-c0b5664f7fdb/ProjectsForFun/Pool/repos"
docker run -v $REPOS/billiards-shots-api/src/math/orient_pocket_gen:/home/sage/output -it sagemath/sagemath:latest


expand((a2 - r^2) * (a2 - 2 * t * av + t^2 * v2) ** 2
	+ 2 * (t * av - a2) * (a2 - t * av) * (a2 - 2 * t * av + t^2 * v2)
	+ (a2 - 2 * t * av + t^2 * v2) * (a2 - t * av)^2
	- r^2 * (a2 - 2 * t * av + t^2 * v2) ** 2).collect(t)

a2, av, v2, t, r = var('a2 av v2 t r')
s = (a2 - t * av) / (a2 - 2 * t * av + t^2 * v2)
sols = solve([(1 - s)^2 * a2 + s * t * av - s^2 * t * av + s^2 * t^2 * v2 == r^2], t)

a2, av, v2, s, t, r = var('a2 av v2 s t r')
sols = solve([
	 (a2 - 2 * t * av + t^2 * v2) * s == (a2 - t * av),
	 (1 - s)^2 * a2 + s * t * av - s^2 * t * av + s^2 * t^2 * v2 == r^2,
], s, t)

from sympy.utilities.codegen import codegen
for idx, sol in enumerate(sols):
	name = "gen_sol_" + str(idx)
	for filename, code in codegen((name, sols[0]), "C", name, header=True, empty=False):
		with open('/home/sage/output/' + filename, 'w') as outfile:
			outfile.write(code)
	 */

	[[nodiscard]] inline
	const geometry::MaybePoint orient_pocket(
		const geometry::MaybePoint& source,
		const geometry::MaybePoint& segment1,
		const geometry::MaybePoint& segment2,
		const geometry::MaybeDouble radius
	) {
		if (!source.is_valid() || !segment1.is_valid() || !segment2.is_valid() || !radius.is_valid()) {
			return geometry::MaybePoint{};
		}

		const geometry::Point v = (segment2 - segment1).get();
		const geometry::Point a = (source - segment1).get();

		const double a2 = a.norm2();
		const double v2 = v.norm2();
		const double av = a.dot(v);

		const double r = radius.get();
		const double r2 = r * r;

		if (v.dot(a) > 0) {
			// TODO: Maybe it goes though outer but not inner...
			return segment1 + v * r / std::sqrt(v2);
		}

		std::list<double> sols;

		const double c0 = -a2 * a2 * r * r;
		const double c1 = 4 * a2 * av * r * r;
		const double c2 = -(4 * av * av * r * r + 2 * a2 * r * r * v2 + a2 * av * av - a2 * a2 * v2);
		const double c3 = 2 * (2 * av * r * r * v2 + av * av * av - a2 * av * v2);
		const double c4 = -(r * r * v2 * v2 + av * av * v2 - a2 * v2 * v2);

		billiards::math::solve_4(
			c0, c1, c2, c3, c4,
			0.5 * std::sqrt(v2),
			[&](const double t) {
//				if (std::abs(t*t*v2 - 2*av*t + a2) < TOLERANCE) {
//					return;
//				}
//				const double s = -(av * t - a2) / (t * t * v2 - 2 * av * t + a2);
//				const double d = (t * t * v2 - 2 * av * t + a2) * s * s + 2 * (av * t - a2) * s + a2;
//				std::cout << "t=" << t << ", d=" << d << ", r2=" << r2 << std::endl;
				sols.push_back(t);
			}
		);

#if 0
		for (const double& t: sols) {
			std::cout << "t=" << t << std::endl;
			const double s = -(av * t - a2) / (t * t * v2 - 2 * av * t + a2);
			std::cout << "s=" << s << std::endl;
			const geometry::MaybePoint closest = segment1 + (a * (1 - s) + v * s * t);
			std::cout << "closest=" << closest << std::endl;
			std::cout << "distance=" << (closest - segment1).norm() << std::endl;
			std::cout << "value=" << (
				(t*t*v2 - 2*av*t + a2)*s*s + 2*(av*t - a2)*s + a2
			) << std::endl;

			std::cout << "expected distance=" << std::pow((
				std::pow(((1 - s) * a.x + s * t * v.x), 2) +
				std::pow(((1 - s) * a.y + s * t * v.y), 2)
			), 0.5) << std::endl;


			std::cout << "expected distance 2 =" << std::pow((
				std::pow((1 - s), 2) * a2 + (1 - s) * s * t * av
					+ s * s * t * t * v2
			), 0.5) << std::endl;

			std::cout << "==================================" << std::endl;
		}
#endif

		for (const double& t : sols) {
			if (t >= 0 && t * t <= v2) {
#if 0
				std::cout << "t=" << t << std::endl;
				const double s = (a2 - 0.5 * t * av) / (a2 - t * av + t*t * v2);
				std::cout << "s=" << s << std::endl;
				const geometry::MaybePoint closest = segment1 + (a * (1 - 2) + v * t);
				std::cout << "closest=" << closest << std::endl;
				std::cout << "distance=" << (closest - segment1).norm() << std::endl;
#endif
				return segment1 + v * t;
			}
		}
		return geometry::MaybePoint{};
	}
}

//# if 1
//gen_sol_0(a2, av, r, v2, &t);
//if (!std::isnan(t) && !std::isinf(t)) {
//sols.push_back(t);
//}
//gen_sol_1(a2, av, r, v2, &t);
//if (!std::isnan(t) && !std::isinf(t)) {
//sols.push_back(t);
//}
//gen_sol_2(a2, av, r, v2, &t);
//if (!std::isnan(t) && !std::isinf(t)) {
//sols.push_back(t);
//}
//gen_sol_3(a2, av, r, v2, &t);
//if (!std::isnan(t) && !std::isinf(t)) {
//sols.push_back(t);
//}
//#else
//double t;
//		bool success;
//		success = billiards::gen::orient_pocket::get_solution_000(r, a2, v2, av, &t);
//		if (success) {
//			sols.push_back(t);
//		}
//		success = billiards::gen::orient_pocket::get_solution_001(r, a2, v2, av, &t);
//		if (success) {
//			sols.push_back(t);
//		}
//		success = billiards::gen::orient_pocket::get_solution_002(r, a2, v2, av, &t);
//		if (success) {
//			sols.push_back(t);
//		}
//		success = billiards::gen::orient_pocket::get_solution_003(r, a2, v2, av, &t);
//		if (success) {
//			sols.push_back(t);
//		}
//#endif

//		const double denominator = av2 + 4 * (r2 - a2) * v2;
//		if (std::abs(denominator) < TOLERANCE) {
//			// TODO:
//			return geometry::MaybePoint{};
//		}
//		const double discriminant = av2 * r2 - a2 * av2 - 4 * (a2 * r2 - a22) * v2;
//		if (discriminant < 0) {
//			// TODO:
//			return geometry::MaybePoint{};
//		}



//std::list<double> sols;
//if (discriminant < TOLERANCE) {
//const double t = 2 * av * r2 / denominator;
//sols.push_back(t);
//} else {
//const double sd = std::sqrt(discriminant);
//sols.push_back(2 * (av * r2 + sd * r) / denominator);
//sols.push_back(2 * (av * r2 - sd * r) / denominator);
//}
//
//for (const double& t: sols) {
//std::cout << "t=" << t << std::endl;
//const double s = (a2 - 0.5 * t * av) / (a2 - t * av + t * t * v2);
//std::cout << "s=" << s << std::endl;
//const geometry::MaybePoint closest = segment1 + (a * (1 - s) + v * s * t);
//std::cout << "closest=" << closest << std::endl;
//std::cout << "distance=" << (closest - segment1).norm() << std::endl;
//std::cout << "value=" << (
//std::pow((1 - s), 2) * a2 + s * t * av - s * s * t * av + s * s * t * t * v2
//) << std::endl;
//
//std::cout << "expected distance=" << std::pow((
//	std::pow(((1 - s) * a.x + s * t * v.x), 2) +
//std::pow(((1 - s) * a.y + s * t * v.y), 2)
//), 0.5) << std::endl;
//
//
//std::cout << "expected distance 2 =" << std::pow((
//	std::pow((1 - s), 2) * a2 + (1 - s) * s * t * av
//+ s * s * t * t * v2
//), 0.5) << std::endl;
//
//std::cout << "==================================" << std::endl;
//}
//
//for (const double& t : sols) {
//if (t >= 0 && t * t <= v2) {
//std::cout << "t=" << t << std::endl;
//const double s = (a2 - 0.5 * t * av) / (a2 - t * av + t*t * v2);
//std::cout << "s=" << s << std::endl;
//const geometry::MaybePoint closest = segment1 + (a * (1 - 2) + v * t);
//std::cout << "closest=" << closest << std::endl;
//std::cout << "distance=" << (closest - segment1).norm() << std::endl;
//
//return segment1 + v * t;
//}
//}
//return geometry::MaybePoint{};


#endif //IDEA_ORIENT_POCKET_H
