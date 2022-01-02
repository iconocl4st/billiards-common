//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_ROLLING_GLANCE_UNKNOWN_DEST_H
#define IDEA_ROLLING_GLANCE_UNKNOWN_DEST_H

#include "billiards_common/shots/RollingGlanceCalculation.h"
#include "billiards_common/math/quad_systems.h"

namespace billiards::shots::math {

/*
export REPOS=/mnt/1f0ab4b3-c472-49e1-92d8-c0b5664f7fdb/ProjectsForFun/Pool/repos
docker run -v $(realpath $REPOS/billiards-scripts/sage/scripts):/home/sage/scripts --workdir=/home/sage/scripts -it sagemath/sagemath:latest

 */

	class KissToPocketSolution {
	public:
		double x, y;
		double dx, dy;
		double tx, ty;
		double ax, ay;
		KissToPocketSolution(
			const double x, const double y,
			const double dx, const double dy,
			const double tx, const double ty,
			const double ax, const double ay
		)
			: x{x}, y{y}
			, dx{dx}, dy{dy}
			, tx{tx}, ty{ty}
			, ax{ax}, ay{ay}
		{}

		[[nodiscard]] inline
		billiards::shots::RollingGlanceCalculation calc() const {
			return billiards::shots::RollingGlanceCalculation{
				x, y,
				tx, ty,
				ax, ay
			};
		}

		[[nodiscard]] inline
		bool same_as(const KissToPocketSolution& other) const {
			return (
				std::abs(x - other.x) < TOLERANCE &&
				std::abs(y - other.y) < TOLERANCE &&
				std::abs(dx - other.dx) < TOLERANCE &&
				std::abs(dy - other.dy) < TOLERANCE
			);
		}
	};

	void solve_glance_and_pocket(
		const double ax, const double ay,
		const double px, const double py,
		const double opx, const double opy,
		const double r1, const double r2,
		const std::function<void(const KissToPocketSolution& sol)>& receiver
	) {
		const double px2 = px * px;
		const double px3 = px2 * px;
		const double px4 = px3 * px;
		const double py2 = py * py;
		const double py3 = py2 * py;
		const double py4 = py3 * py;
		const double r12 = r1 * r1;
		const double r13 = r12 * r1;
		const double r14 = r13 * r1;
		const double r22 = r2 * r2;
		const double r23 = r22 * r2;
		const double r24 = r23 * r2;
		const double ax2 = ax * ax;
		const double ay2 = ay * ay;
		const double px2ppy2 = px2 + py2;
		const double ax2pay2 = ax2 + ay2;
		const double radi = px2ppy2 - r12;
		if (radi < 0) {
			// The pocket is too close to the object ball
			return;
		}
		const double sradi = std::sqrt(radi);
		if (px2ppy2 < TOL) {
			// The object is at the pocket
			return;
		}
		if (ax2pay2 < TOL) {
			// the cue is at the object
			return;
		}

		std::list<KissToPocketSolution> unique_sols;
		const auto unique_checker = [&](const KissToPocketSolution& g) {
			for (const KissToPocketSolution& prev: unique_sols) {
				if (prev.same_as(g)) {
					return;
				}
			}
			unique_sols.push_back(g);
			receiver(g);
		};
		const auto check_sol = [&](const KissToPocketSolution& g) {
			unique_checker(g);
		};

		const auto print_receiver = [&](const KissToPocketSolution& ktps) {
			std::cout << "found solution x=" << ktps.x << ", y=" << ktps.y << std::endl;
			std::cout << "radius: " << std::sqrt(ktps.x * ktps.x + ktps.y * ktps.y) << std::endl;

			std::cout << "ax=" << ax << ", ay=" << ay << std::endl;
			const double s1 = (ay * ktps.x - ax * ktps.y) / (ax * ax + ay * ay);
			std::cout << "s1 = " << s1 << std::endl;

			const double alpha = 2/7.0;
			const double v1 = (1 - alpha) * ktps.tx + alpha * s1 * ax;
			const double v2 = (1 - alpha) * ktps.tx + alpha * s1 * ax;

			const double v3 = ktps.dx * ax + ktps.dy * ay;
			std::cout << "another thing: " << v3 << std::endl;

//			if (std::abs(v3 - (195.378)) > 10) {
//				return;
//			}

			std::cout << "The ratios: " << std::endl;
			std::cout << v1 << std::endl;
			std::cout << v2 << std::endl;
			std::cout << v1 / v2 << std::endl;
			std::cout << ktps.dx / ktps.dy << std::endl;

			if (ktps.x * px + ktps.y * py < 0) {
				return;
			}
			if (ktps.x * ax + ktps.y * ay < 0) {
				return;
			}
			if (ktps.ax * ktps.x + ktps.ay * ktps.y < 0) {
				return;
			}
			check_sol(ktps);
		};

		{
			const double a00 = -r12 - 2*r1*r2 - r22;
			const double a10 = 0;
			const double a01 = 0;
			const double a20 = 1;
			const double a11 = 0;
			const double a02 = 1;
			const double b00 = 0;
			const double b10 = 5/(double) 14*std::pow(ax, 2)*px - 1/(double) 7*ax*ay*px + 1/(double) 2*std::pow(ay, 2)*px - 5/(double) 14*std::pow(ax, 2)*py - 1/(double) 7*ax*ay*py - 1/(double) 2*std::pow(ay, 2)*py;
			const double b01 = 1/(double) 2*std::pow(ax, 2)*px - 1/(double) 7*ax*ay*px + 5/(double) 14*std::pow(ay, 2)*px + 1/(double) 2*std::pow(ax, 2)*py + 1/(double) 7*ax*ay*py + 5/(double) 14*std::pow(ay, 2)*py;
			const double b20 = -5/(double) 14*std::pow(ax, 2) + 1/(double) 7*ax*ay - 1/(double) 2*std::pow(ay, 2);
			const double b11 = -1/(double) 7*std::pow(ax, 2) + 2/(double) 7*ax*ay + 1/(double) 7*std::pow(ay, 2);
			const double b02 = -1/(double) 2*std::pow(ax, 2) - 1/(double) 7*ax*ay - 5/(double) 14*std::pow(ay, 2);
			const auto get_glance = [&](const double x, const double y) {
				return KissToPocketSolution{
					x, y,
					1/(double) 2*px - 1/(double) 2*py - 1/(double) 2*x + 1/(double) 2*y, 1/(double) 2*px + 1/(double) 2*py - 1/(double) 2*x - 1/(double) 2*y,
					y, -x,
					ax - x, ay - y
				};
			};
			billiards::math::solve_22_22(
				a00, a10, a01, a20, a11, a02,
				b00, b10, b01, b20, b11, b02,
				M_PI, M_PI,
				[&](const double x, const double y) {
					if (px * y - py * x > 0) {
						return;
					}
					print_receiver(get_glance(x, y));
				}
			);
		}

		// if px * y - py * x < 0
		{
			const double a00 = -r12 - 2*r1*r2 - r22;
			const double a10 = 0;
			const double a01 = 0;
			const double a20 = 1;
			const double a11 = 0;
			const double a02 = 1;
			const double b00 = 0;
			const double b10 = -5/(double) 14*std::pow(ax, 2)*px - 1/(double) 7*ax*ay*px - 1/(double) 2*std::pow(ay, 2)*px - 5/(double) 14*std::pow(ax, 2)*py + 1/(double) 7*ax*ay*py - 1/(double) 2*std::pow(ay, 2)*py;
			const double b01 = 1/(double) 2*std::pow(ax, 2)*px + 1/(double) 7*ax*ay*px + 5/(double) 14*std::pow(ay, 2)*px - 1/(double) 2*std::pow(ax, 2)*py + 1/(double) 7*ax*ay*py - 5/(double) 14*std::pow(ay, 2)*py;
			const double b20 = 2/(double) 7*ax*ay;
			const double b11 = -2/(double) 7*std::pow(ax, 2) + 2/(double) 7*std::pow(ay, 2);
			const double b02 = -2/(double) 7*ax*ay;
			const auto get_glance = [&](const double x, const double y) {
				return KissToPocketSolution{
					x, y,
					1/(double) 2*px + 1/(double) 2*py - y, -1/(double) 2*px + 1/(double) 2*py + x,
					-y, x,
					ax - x, ay - y
				};
			};
			billiards::math::solve_22_22(
				a00, a10, a01, a20, a11, a02,
				b00, b10, b01, b20, b11, b02,
				M_PI, M_PI,
				[&](const double x, const double y) {
					if (px * y - py * x < 0) {
						return;
					}
					print_receiver(get_glance(x, y));
				}
			);
		}

	}


	void solve_glance_and_pocket(
		const geometry::MaybePoint& cue_loc,
		const geometry::MaybePoint& pocket_loc,
		const geometry::MaybePoint& other_pocket_loc,
		const double r1, const double r2,
		const std::function<void(const KissToPocketSolution& sol)>& receiver
	) {
		if (!cue_loc.is_valid() || !pocket_loc.is_valid() || !other_pocket_loc.is_valid()) {
			return;
		}
		solve_glance_and_pocket(
			cue_loc.x.get(), cue_loc.y.get(),
			pocket_loc.x.get(), pocket_loc.y.get(),
			other_pocket_loc.x.get(), other_pocket_loc.y.get(),
			r1, r2,
			receiver
		);
	}
}


/*
 *

		{
			const double a00 = -r12 - 2*r1*r2 - r22;
			const double a10 = 0;
			const double a01 = 0;
			const double a20 = 1;
			const double a11 = 0;
			const double a02 = 1;
			const double b00 = 0;
			const double b10 = 5/(double) 14*std::pow(ax, 2)*px - 1/(double) 7*ax*ay*px + 1/(double) 2*std::pow(ay, 2)*px - 5/(double) 14*std::pow(ax, 2)*py - 1/(double) 7*ax*ay*py - 1/(double) 2*std::pow(ay, 2)*py;
			const double b01 = 1/(double) 2*std::pow(ax, 2)*px - 1/(double) 7*ax*ay*px + 5/(double) 14*std::pow(ay, 2)*px + 1/(double) 2*std::pow(ax, 2)*py + 1/(double) 7*ax*ay*py + 5/(double) 14*std::pow(ay, 2)*py;
			const double b20 = -5/(double) 14*std::pow(ax, 2) + 1/(double) 7*ax*ay - 1/(double) 2*std::pow(ay, 2);
			const double b11 = -1/(double) 7*std::pow(ax, 2) + 2/(double) 7*ax*ay + 1/(double) 7*std::pow(ay, 2);
			const double b02 = -1/(double) 2*std::pow(ax, 2) - 1/(double) 7*ax*ay - 5/(double) 14*std::pow(ay, 2);

			billiards::math::solve_22_22(
				a00, a10, a01, a20, a11, a02,
				b00, b10, b01, b20, b11, b02,
				M_PI, M_PI,
				print_receiver
			);
		}
		{
			const double a00 = -r12 - 2*r1*r2 - r22;
			const double a10 = 0;
			const double a01 = 0;
			const double a20 = 1;
			const double a11 = 0;
			const double a02 = 1;
			const double b00 = 0;
			const double b10 = -5/(double) 14*std::pow(ax, 2)*px - 1/(double) 7*ax*ay*px - 1/(double) 2*std::pow(ay, 2)*px - 5/(double) 14*std::pow(ax, 2)*py + 1/(double) 7*ax*ay*py - 1/(double) 2*std::pow(ay, 2)*py;
			const double b01 = 1/(double) 2*std::pow(ax, 2)*px + 1/(double) 7*ax*ay*px + 5/(double) 14*std::pow(ay, 2)*px - 1/(double) 2*std::pow(ax, 2)*py + 1/(double) 7*ax*ay*py - 5/(double) 14*std::pow(ay, 2)*py;
			const double b20 = 2/(double) 7*ax*ay;
			const double b11 = -2/(double) 7*std::pow(ax, 2) + 2/(double) 7*std::pow(ay, 2);
			const double b02 = -2/(double) 7*ax*ay;

		}
		const auto print_receiver = [&](const double x, const double y) {
			std::cout << "found solution x=" << x << ", y=" << y << std::endl;
			std::cout << "radius: " << std::sqrt(x * x + y * y) << std::endl;

			check_sol(KissToPocketSolution{
				x, y,
				1/2*px + 1/2*py - y, -1/2*px + 1/2*py + x,
				y, -x,
				ax - x, ay - y
			});
		};
		const auto x_receiver = [&](const double x) {
			// if px * y - py * x > 0
			// tangent direction (tx, ty) = [y, -x]
			// destination direction (dx, dy) = (1/2*px - 1/2*py + 1/2*x + 1/2*y,1/2*px + 1/2*py - 1/2*x + 1/2*y)
			// computed orthogonal requirement: s1=-(ay*x - ax*y)/(ax^2 + ay^2)
			// computed rolling requirement: poly_d=5*ax^2*px*x - 2*ax*ay*px*x + 7*ay^2*px*x - 5*ax^2*py*x - 2*ax*ay*py*x - 7*ay^2*py*x + 5*ax^2*x^2 + 2*ax*ay*x^2 + 7*ay^2*x^2 + 7*ax^2*px*y - 2*ax*ay*px*y + 5*ay^2*px*y + 7*ax^2*py*y + 2*ax*ay*py*y + 5*ay^2*py*y - 2*ax^2*x*y - 4*ax*ay*x*y + 2*ay^2*x*y + 7*ax^2*y^2 - 2*ax*ay*y^2 + 5*ay^2*y^2
			// first resultant polynomial: 1/4*px^4 + 1/4*py^4 + 1/4*r1^4 - r1^3*r2 + 1/2*r1^2*r2^2 + r1*r2^3 + 1/4*r2^4 - px^3*x + 1/2*(py^2 - r1^2 + 2*r1*r2 + r2^2 + 2*x^2)*px^2 - 1/2*(3*r1^2 + 2*r1*r2 + r2^2 - 2*x^2)*py^2 - (py^2*x - (r1^2 - 2*r1*r2 - r2^2)*x)*px
			// second resultant polynomial: 1/16*px^8 + 1/16*py^8 + 1/16*r1^8 - 1/2*r1^7*r2 + 1/4*px^6*r2^2 + 3/8*px^4*r2^4 + 1/4*px^2*r2^6 + 1/16*r2^8 + 1/4*(px^2 - 3*r1^2 - 2*r1*r2 - r2^2)*py^6 - 1/4*(px^2 - 5*r2^2)*r1^6 + 1/2*(3*px^2*r2 - r2^3)*r1^5 + 1/8*(3*px^4 + 19*r1^4 + 20*r1^3*r2 - 2*px^2*r2^2 + 3*r2^4 - 2*(7*px^2 - 11*r2^2)*r1^2 - 4*(px^2*r2 - 3*r2^3)*r1)*py^4 + 1/8*(3*px^4 - 18*px^2*r2^2 - 13*r2^4)*r1^4 + (px^4 + 2*px^2*py^2 + py^4)*x^4 - 1/2*(3*px^4*r2 + 2*px^2*r2^3 - r2^5)*r1^3 - 2*(px^5 + px*py^4 - px^3*r1^2 + 2*px^3*r1*r2 + px^3*r2^2 + (2*px^3 - px*r1^2 + 2*px*r1*r2 + px*r2^2)*py^2)*x^3 + 1/4*(px^6 - 3*r1^6 + 10*r1^5*r2 + px^4*r2^2 - px^2*r2^4 - r2^6 + (7*px^2 + r2^2)*r1^4 - 12*(px^2*r2 + r2^3)*r1^3 - (5*px^4 + 10*px^2*r2^2 + 13*r2^4)*r1^2 + 2*(px^4*r2 - 2*px^2*r2^3 - 3*r2^5)*r1)*py^2 - 1/4*(px^6 - 3*px^4*r2^2 - 9*px^2*r2^4 - 5*r2^6)*r1^2 + 1/2*(3*px^6 + py^6 + 3*px^2*r1^4 - 12*px^2*r1^3*r2 + 6*px^4*r2^2 + 3*px^2*r2^4 + (5*px^2 - 6*r1^2 - 4*r1*r2 - 2*r2^2)*py^4 + (7*px^4 + r1^4 - 4*r1^3*r2 + 4*px^2*r2^2 + r2^4 - 2*(6*px^2 - r2^2)*r1^2 + 4*(2*px^2*r2 + r2^3)*r1)*py^2 - 6*(px^4 - px^2*r2^2)*r1^2 + 12*(px^4*r2 + px^2*r2^3)*r1)*x^2 + 1/2*(px^6*r2 + 3*px^4*r2^3 + 3*px^2*r2^5 + r2^7)*r1 - 1/2*(px^7 + px*py^6 - px*r1^6 + 6*px*r1^5*r2 + 3*px^5*r2^2 + 3*px^3*r2^4 + px*r2^6 + (3*px^3 - 7*px*r1^2 - 2*px*r1*r2 - px*r2^2)*py^4 + 3*(px^3 - 3*px*r2^2)*r1^4 - 4*(3*px^3*r2 + px*r2^3)*r1^3 + (3*px^5 + 7*px*r1^4 - 12*px*r1^3*r2 + 2*px^3*r2^2 - px*r2^4 - 10*(px^3 + px*r2^2)*r1^2 + 4*(px^3*r2 - px*r2^3)*r1)*py^2 - 3*(px^5 - 2*px^3*r2^2 - 3*px*r2^4)*r1^2 + 6*(px^5*r2 + 2*px^3*r2^3 + px*r2^5)*r1)*x
			// final resultant polynomial: 1/16*(px^4 + 2*px^2*py^2 + py^4 - 2*px^2*r1^2 - 6*py^2*r1^2 + r1^4 + 4*px^2*r1*r2 - 4*py^2*r1*r2 - 4*r1^3*r2 + 2*px^2*r2^2 - 2*py^2*r2^2 + 2*r1^2*r2^2 + 4*r1*r2^3 + r2^4 - 4*px^3*x - 4*px*py^2*x + 4*px*r1^2*x - 8*px*r1*r2*x - 4*px*r2^2*x + 4*px^2*x^2 + 4*py^2*x^2)^2

			const double y = std::sqrt(std::pow((r1 + r2), 2) - x * x);
			// TODO: move dx, dy to be along the correct line...
			check_sol(KissToPocketSolution{
				x, y,
				0.5 * (px - py + x + y), 0.5 * (px + py - x + y),
				y, -x,
				ax - x, ay - y
			});
		};

 */
#endif //IDEA_ROLLING_GLANCE_UNKNOWN_DEST_H
