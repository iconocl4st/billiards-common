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
		const double r1_2 = r1 * r1;
		const double r1_3 = r1_2 * r1;
		const double r1_4 = r1_3 * r1;
		const double r1_5 = r1_4 * r1;
		const double r1_6 = r1_5 * r1;
		const double r1_7 = r1_6 * r1;
		const double r1_8 = r1_7 * r1;
		const double r2_2 = r2 * r2;
		const double r2_3 = r2_2 * r2;
		const double r2_4 = r2_3 * r2;
		const double r2_5 = r2_4 * r2;
		const double r2_6 = r2_5 * r2;
		const double r2_7 = r2_6 * r2;
		const double r2_8 = r2_7 * r2;
		const double ax_2 = ax * ax;
		const double ax_3 = ax_2 * ax;
		const double ax_4 = ax_3 * ax;
		const double ay_2 = ay * ay;
		const double ay_3 = ay_2 * ay;
		const double ay_4 = ay_3 * ay;
		const double px_2 = px * px;
		const double py_2 = py * py;

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
			const double a0 = -49*ax_4*px_2*r1_2 + 70*ax_3*ay*px_2*r1_2 - 53*ax_2*ay_2*px_2*r1_2 + 20*ax*ay_3*px_2*r1_2 - 4*ay_4*px_2*r1_2 - 98*ax_4*px*py*r1_2 - 6*ax_2*ay_2*px*py*r1_2 - 8*ay_4*px*py*r1_2 - 49*ax_4*py_2*r1_2 - 70*ax_3*ay*py_2*r1_2 - 53*ax_2*ay_2*py_2*r1_2 - 20*ax*ay_3*py_2*r1_2 - 4*ay_4*py_2*r1_2 + 49*ax_4*r1_4 + 70*ax_3*ay*r1_4 + 53*ax_2*ay_2*r1_4 + 20*ax*ay_3*r1_4 + 4*ay_4*r1_4 - 140*ax_3*px*r1_4 + 40*ax*ay_2*px*r1_4 - 3*ax_2*px_2*r1_4 - 20*ax*ay*px_2*r1_4 + 8*ay_2*px_2*r1_4 - 106*ax_2*px*py*r1_4 + 16*ay_2*px*py*r1_4 - 3*ax_2*py_2*r1_4 + 20*ax*ay*py_2*r1_4 + 8*ay_2*py_2*r1_4 + 3*ax_2*r1_6 - 20*ax*ay*r1_6 - 8*ay_2*r1_6 - 40*ax*px*r1_6 - 4*px_2*r1_6 - 8*px*py*r1_6 - 4*py_2*r1_6 + 4*r1_8 - 98*ax_4*px_2*r1*r2 + 140*ax_3*ay*px_2*r1*r2 - 106*ax_2*ay_2*px_2*r1*r2 + 40*ax*ay_3*px_2*r1*r2 - 8*ay_4*px_2*r1*r2 - 196*ax_4*px*py*r1*r2 - 12*ax_2*ay_2*px*py*r1*r2 - 16*ay_4*px*py*r1*r2 - 98*ax_4*py_2*r1*r2 - 140*ax_3*ay*py_2*r1*r2 - 106*ax_2*ay_2*py_2*r1*r2 - 40*ax*ay_3*py_2*r1*r2 - 8*ay_4*py_2*r1*r2 + 196*ax_4*r1_3*r2 + 280*ax_3*ay*r1_3*r2 + 212*ax_2*ay_2*r1_3*r2 + 80*ax*ay_3*r1_3*r2 + 16*ay_4*r1_3*r2 - 560*ax_3*px*r1_3*r2 + 160*ax*ay_2*px*r1_3*r2 - 12*ax_2*px_2*r1_3*r2 - 80*ax*ay*px_2*r1_3*r2 + 32*ay_2*px_2*r1_3*r2 - 424*ax_2*px*py*r1_3*r2 + 64*ay_2*px*py*r1_3*r2 - 12*ax_2*py_2*r1_3*r2 + 80*ax*ay*py_2*r1_3*r2 + 32*ay_2*py_2*r1_3*r2 + 18*ax_2*r1_5*r2 - 120*ax*ay*r1_5*r2 - 48*ay_2*r1_5*r2 - 240*ax*px*r1_5*r2 - 24*px_2*r1_5*r2 - 48*px*py*r1_5*r2 - 24*py_2*r1_5*r2 + 32*r1_7*r2 - 49*ax_4*px_2*r2_2 + 70*ax_3*ay*px_2*r2_2 - 53*ax_2*ay_2*px_2*r2_2 + 20*ax*ay_3*px_2*r2_2 - 4*ay_4*px_2*r2_2 - 98*ax_4*px*py*r2_2 - 6*ax_2*ay_2*px*py*r2_2 - 8*ay_4*px*py*r2_2 - 49*ax_4*py_2*r2_2 - 70*ax_3*ay*py_2*r2_2 - 53*ax_2*ay_2*py_2*r2_2 - 20*ax*ay_3*py_2*r2_2 - 4*ay_4*py_2*r2_2 + 294*ax_4*r1_2*r2_2 + 420*ax_3*ay*r1_2*r2_2 + 318*ax_2*ay_2*r1_2*r2_2 + 120*ax*ay_3*r1_2*r2_2 + 24*ay_4*r1_2*r2_2 - 840*ax_3*px*r1_2*r2_2 + 240*ax*ay_2*px*r1_2*r2_2 - 18*ax_2*px_2*r1_2*r2_2 - 120*ax*ay*px_2*r1_2*r2_2 + 48*ay_2*px_2*r1_2*r2_2 - 636*ax_2*px*py*r1_2*r2_2 + 96*ay_2*px*py*r1_2*r2_2 - 18*ax_2*py_2*r1_2*r2_2 + 120*ax*ay*py_2*r1_2*r2_2 + 48*ay_2*py_2*r1_2*r2_2 + 45*ax_2*r1_4*r2_2 - 300*ax*ay*r1_4*r2_2 - 120*ay_2*r1_4*r2_2 - 600*ax*px*r1_4*r2_2 - 60*px_2*r1_4*r2_2 - 120*px*py*r1_4*r2_2 - 60*py_2*r1_4*r2_2 + 112*r1_6*r2_2 + 196*ax_4*r1*r2_3 + 280*ax_3*ay*r1*r2_3 + 212*ax_2*ay_2*r1*r2_3 + 80*ax*ay_3*r1*r2_3 + 16*ay_4*r1*r2_3 - 560*ax_3*px*r1*r2_3 + 160*ax*ay_2*px*r1*r2_3 - 12*ax_2*px_2*r1*r2_3 - 80*ax*ay*px_2*r1*r2_3 + 32*ay_2*px_2*r1*r2_3 - 424*ax_2*px*py*r1*r2_3 + 64*ay_2*px*py*r1*r2_3 - 12*ax_2*py_2*r1*r2_3 + 80*ax*ay*py_2*r1*r2_3 + 32*ay_2*py_2*r1*r2_3 + 60*ax_2*r1_3*r2_3 - 400*ax*ay*r1_3*r2_3 - 160*ay_2*r1_3*r2_3 - 800*ax*px*r1_3*r2_3 - 80*px_2*r1_3*r2_3 - 160*px*py*r1_3*r2_3 - 80*py_2*r1_3*r2_3 + 224*r1_5*r2_3 + 49*ax_4*r2_4 + 70*ax_3*ay*r2_4 + 53*ax_2*ay_2*r2_4 + 20*ax*ay_3*r2_4 + 4*ay_4*r2_4 - 140*ax_3*px*r2_4 + 40*ax*ay_2*px*r2_4 - 3*ax_2*px_2*r2_4 - 20*ax*ay*px_2*r2_4 + 8*ay_2*px_2*r2_4 - 106*ax_2*px*py*r2_4 + 16*ay_2*px*py*r2_4 - 3*ax_2*py_2*r2_4 + 20*ax*ay*py_2*r2_4 + 8*ay_2*py_2*r2_4 + 45*ax_2*r1_2*r2_4 - 300*ax*ay*r1_2*r2_4 - 120*ay_2*r1_2*r2_4 - 600*ax*px*r1_2*r2_4 - 60*px_2*r1_2*r2_4 - 120*px*py*r1_2*r2_4 - 60*py_2*r1_2*r2_4 + 280*r1_4*r2_4 + 18*ax_2*r1*r2_5 - 120*ax*ay*r1*r2_5 - 48*ay_2*r1*r2_5 - 240*ax*px*r1*r2_5 - 24*px_2*r1*r2_5 - 48*px*py*r1*r2_5 - 24*py_2*r1*r2_5 + 224*r1_3*r2_5 + 3*ax_2*r2_6 - 20*ax*ay*r2_6 - 8*ay_2*r2_6 - 40*ax*px*r2_6 - 4*px_2*r2_6 - 8*px*py*r2_6 - 4*py_2*r2_6 + 112*r1_2*r2_6 + 32*r1*r2_7 + 4*r2_8;
			const double a1 = 42*ax_4*px*r1_2 - 140*ax_3*ay*px*r1_2 - 6*ax_2*ay_2*px*r1_2 - 40*ax*ay_3*px*r1_2 - 48*ay_4*px*r1_2 + 146*ax_3*px_2*r1_2 - 30*ax_2*ay*px_2*r1_2 + 56*ax*ay_2*px_2*r1_2 - 20*ay_3*px_2*r1_2 + 98*ax_4*py*r1_2 + 6*ax_2*ay_2*py*r1_2 + 8*ay_4*py*r1_2 + 212*ax_3*px*py*r1_2 + 192*ax*ay_2*px*py*r1_2 + 146*ax_3*py_2*r1_2 + 30*ax_2*ay*py_2*r1_2 + 56*ax*ay_2*py_2*r1_2 + 20*ay_3*py_2*r1_2 - 6*ax_3*r1_4 - 30*ax_2*ay*r1_4 - 96*ax*ay_2*r1_4 - 20*ay_3*r1_4 + 114*ax_2*px*r1_4 + 40*ax*ay*px*r1_4 + 56*ay_2*px*r1_4 + 56*ax*px_2*r1_4 + 20*ay*px_2*r1_4 + 106*ax_2*py*r1_4 - 16*ay_2*py*r1_4 + 32*ax*px*py*r1_4 + 56*ax*py_2*r1_4 - 20*ay*py_2*r1_4 - 16*ax*r1_6 + 20*ay*r1_6 - 8*px*r1_6 + 8*py*r1_6 + 84*ax_4*px*r1*r2 - 280*ax_3*ay*px*r1*r2 - 12*ax_2*ay_2*px*r1*r2 - 80*ax*ay_3*px*r1*r2 - 96*ay_4*px*r1*r2 + 292*ax_3*px_2*r1*r2 - 60*ax_2*ay*px_2*r1*r2 + 112*ax*ay_2*px_2*r1*r2 - 40*ay_3*px_2*r1*r2 + 196*ax_4*py*r1*r2 + 12*ax_2*ay_2*py*r1*r2 + 16*ay_4*py*r1*r2 + 424*ax_3*px*py*r1*r2 + 384*ax*ay_2*px*py*r1*r2 + 292*ax_3*py_2*r1*r2 + 60*ax_2*ay*py_2*r1*r2 + 112*ax*ay_2*py_2*r1*r2 + 40*ay_3*py_2*r1*r2 - 24*ax_3*r1_3*r2 - 120*ax_2*ay*r1_3*r2 - 384*ax*ay_2*r1_3*r2 - 80*ay_3*r1_3*r2 + 456*ax_2*px*r1_3*r2 + 160*ax*ay*px*r1_3*r2 + 224*ay_2*px*r1_3*r2 + 224*ax*px_2*r1_3*r2 + 80*ay*px_2*r1_3*r2 + 424*ax_2*py*r1_3*r2 - 64*ay_2*py*r1_3*r2 + 128*ax*px*py*r1_3*r2 + 224*ax*py_2*r1_3*r2 - 80*ay*py_2*r1_3*r2 - 96*ax*r1_5*r2 + 120*ay*r1_5*r2 - 48*px*r1_5*r2 + 48*py*r1_5*r2 + 42*ax_4*px*r2_2 - 140*ax_3*ay*px*r2_2 - 6*ax_2*ay_2*px*r2_2 - 40*ax*ay_3*px*r2_2 - 48*ay_4*px*r2_2 + 146*ax_3*px_2*r2_2 - 30*ax_2*ay*px_2*r2_2 + 56*ax*ay_2*px_2*r2_2 - 20*ay_3*px_2*r2_2 + 98*ax_4*py*r2_2 + 6*ax_2*ay_2*py*r2_2 + 8*ay_4*py*r2_2 + 212*ax_3*px*py*r2_2 + 192*ax*ay_2*px*py*r2_2 + 146*ax_3*py_2*r2_2 + 30*ax_2*ay*py_2*r2_2 + 56*ax*ay_2*py_2*r2_2 + 20*ay_3*py_2*r2_2 - 36*ax_3*r1_2*r2_2 - 180*ax_2*ay*r1_2*r2_2 - 576*ax*ay_2*r1_2*r2_2 - 120*ay_3*r1_2*r2_2 + 684*ax_2*px*r1_2*r2_2 + 240*ax*ay*px*r1_2*r2_2 + 336*ay_2*px*r1_2*r2_2 + 336*ax*px_2*r1_2*r2_2 + 120*ay*px_2*r1_2*r2_2 + 636*ax_2*py*r1_2*r2_2 - 96*ay_2*py*r1_2*r2_2 + 192*ax*px*py*r1_2*r2_2 + 336*ax*py_2*r1_2*r2_2 - 120*ay*py_2*r1_2*r2_2 - 240*ax*r1_4*r2_2 + 300*ay*r1_4*r2_2 - 120*px*r1_4*r2_2 + 120*py*r1_4*r2_2 - 24*ax_3*r1*r2_3 - 120*ax_2*ay*r1*r2_3 - 384*ax*ay_2*r1*r2_3 - 80*ay_3*r1*r2_3 + 456*ax_2*px*r1*r2_3 + 160*ax*ay*px*r1*r2_3 + 224*ay_2*px*r1*r2_3 + 224*ax*px_2*r1*r2_3 + 80*ay*px_2*r1*r2_3 + 424*ax_2*py*r1*r2_3 - 64*ay_2*py*r1*r2_3 + 128*ax*px*py*r1*r2_3 + 224*ax*py_2*r1*r2_3 - 80*ay*py_2*r1*r2_3 - 320*ax*r1_3*r2_3 + 400*ay*r1_3*r2_3 - 160*px*r1_3*r2_3 + 160*py*r1_3*r2_3 - 6*ax_3*r2_4 - 30*ax_2*ay*r2_4 - 96*ax*ay_2*r2_4 - 20*ay_3*r2_4 + 114*ax_2*px*r2_4 + 40*ax*ay*px*r2_4 + 56*ay_2*px*r2_4 + 56*ax*px_2*r2_4 + 20*ay*px_2*r2_4 + 106*ax_2*py*r2_4 - 16*ay_2*py*r2_4 + 32*ax*px*py*r2_4 + 56*ax*py_2*r2_4 - 20*ay*py_2*r2_4 - 240*ax*r1_2*r2_4 + 300*ay*r1_2*r2_4 - 120*px*r1_2*r2_4 + 120*py*r1_2*r2_4 - 96*ax*r1*r2_5 + 120*ay*r1*r2_5 - 48*px*r1*r2_5 + 48*py*r1*r2_5 - 16*ax*r2_6 + 20*ay*r2_6 - 8*px*r2_6 + 8*py*r2_6;
			const double a2 = 53*ax_4*px_2 - 90*ax_3*ay*px_2 + 106*ax_2*ay_2*px_2 - 90*ax*ay_3*px_2 + 53*ay_4*px_2 + 90*ax_4*px*py - 90*ay_4*px*py + 53*ax_4*py_2 + 90*ax_3*ay*py_2 + 106*ax_2*ay_2*py_2 + 90*ax*ay_3*py_2 + 53*ay_4*py_2 - 95*ax_4*r1_2 - 90*ax_3*ay*r1_2 - 100*ax_2*ay_2*r1_2 - 90*ax*ay_3*r1_2 - 5*ay_4*r1_2 + 172*ax_3*px*r1_2 + 60*ax_2*ay*px*r1_2 + 172*ax*ay_2*px*r1_2 + 40*ay_3*px*r1_2 - 135*ax_2*px_2*r1_2 - 40*ax*ay*px_2*r1_2 - 117*ay_2*px_2*r1_2 - 212*ax_3*py*r1_2 - 192*ax*ay_2*py*r1_2 + 58*ax_2*px*py*r1_2 - 22*ay_2*px*py*r1_2 - 135*ax_2*py_2*r1_2 + 40*ax*ay*py_2*r1_2 - 117*ay_2*py_2*r1_2 + 21*ax_2*r1_4 - 40*ax*ay*r1_4 + 61*ay_2*r1_4 + 72*ax*px*r1_4 - 40*ay*px*r1_4 + 8*px_2*r1_4 - 32*ax*py*r1_4 + 8*py_2*r1_4 - 190*ax_4*r1*r2 - 180*ax_3*ay*r1*r2 - 200*ax_2*ay_2*r1*r2 - 180*ax*ay_3*r1*r2 - 10*ay_4*r1*r2 + 344*ax_3*px*r1*r2 + 120*ax_2*ay*px*r1*r2 + 344*ax*ay_2*px*r1*r2 + 80*ay_3*px*r1*r2 - 270*ax_2*px_2*r1*r2 - 80*ax*ay*px_2*r1*r2 - 234*ay_2*px_2*r1*r2 - 424*ax_3*py*r1*r2 - 384*ax*ay_2*py*r1*r2 + 116*ax_2*px*py*r1*r2 - 44*ay_2*px*py*r1*r2 - 270*ax_2*py_2*r1*r2 + 80*ax*ay*py_2*r1*r2 - 234*ay_2*py_2*r1*r2 + 84*ax_2*r1_3*r2 - 160*ax*ay*r1_3*r2 + 244*ay_2*r1_3*r2 + 288*ax*px*r1_3*r2 - 160*ay*px*r1_3*r2 + 32*px_2*r1_3*r2 - 128*ax*py*r1_3*r2 + 32*py_2*r1_3*r2 - 95*ax_4*r2_2 - 90*ax_3*ay*r2_2 - 100*ax_2*ay_2*r2_2 - 90*ax*ay_3*r2_2 - 5*ay_4*r2_2 + 172*ax_3*px*r2_2 + 60*ax_2*ay*px*r2_2 + 172*ax*ay_2*px*r2_2 + 40*ay_3*px*r2_2 - 135*ax_2*px_2*r2_2 - 40*ax*ay*px_2*r2_2 - 117*ay_2*px_2*r2_2 - 212*ax_3*py*r2_2 - 192*ax*ay_2*py*r2_2 + 58*ax_2*px*py*r2_2 - 22*ay_2*px*py*r2_2 - 135*ax_2*py_2*r2_2 + 40*ax*ay*py_2*r2_2 - 117*ay_2*py_2*r2_2 + 126*ax_2*r1_2*r2_2 - 240*ax*ay*r1_2*r2_2 + 366*ay_2*r1_2*r2_2 + 432*ax*px*r1_2*r2_2 - 240*ay*px*r1_2*r2_2 + 48*px_2*r1_2*r2_2 - 192*ax*py*r1_2*r2_2 + 48*py_2*r1_2*r2_2 + 84*ax_2*r1*r2_3 - 160*ax*ay*r1*r2_3 + 244*ay_2*r1*r2_3 + 288*ax*px*r1*r2_3 - 160*ay*px*r1*r2_3 + 32*px_2*r1*r2_3 - 128*ax*py*r1*r2_3 + 32*py_2*r1*r2_3 + 21*ax_2*r2_4 - 40*ax*ay*r2_4 + 61*ay_2*r2_4 + 72*ax*px*r2_4 - 40*ay*px*r2_4 + 8*px_2*r2_4 - 32*ax*py*r2_4 + 8*py_2*r2_4;
			const double a3 = -50*ax_4*px + 180*ax_3*ay*px - 100*ax_2*ay_2*px + 180*ax*ay_3*px - 50*ay_4*px - 162*ax_3*px_2 + 90*ax_2*ay*px_2 - 162*ax*ay_2*px_2 + 90*ay_3*px_2 - 90*ax_4*py + 90*ay_4*py - 180*ax_3*px*py - 180*ax*ay_2*px*py - 162*ax_3*py_2 - 90*ax_2*ay*py_2 - 162*ax*ay_2*py_2 - 90*ay_3*py_2 - 10*ax_3*r1_2 + 90*ax_2*ay*r1_2 - 10*ax*ay_2*r1_2 + 90*ay_3*r1_2 - 162*ax_2*px*r1_2 + 80*ax*ay*px*r1_2 - 162*ay_2*px*r1_2 - 72*ax*px_2*r1_2 - 58*ax_2*py*r1_2 + 22*ay_2*py*r1_2 - 72*ax*py_2*r1_2 - 20*ax_3*r1*r2 + 180*ax_2*ay*r1*r2 - 20*ax*ay_2*r1*r2 + 180*ay_3*r1*r2 - 324*ax_2*px*r1*r2 + 160*ax*ay*px*r1*r2 - 324*ay_2*px*r1*r2 - 144*ax*px_2*r1*r2 - 116*ax_2*py*r1*r2 + 44*ay_2*py*r1*r2 - 144*ax*py_2*r1*r2 - 10*ax_3*r2_2 + 90*ax_2*ay*r2_2 - 10*ax*ay_2*r2_2 + 90*ay_3*r2_2 - 162*ax_2*px*r2_2 + 80*ax*ay*px*r2_2 - 162*ay_2*px*r2_2 - 72*ax*px_2*r2_2 - 58*ax_2*py*r2_2 + 22*ay_2*py*r2_2 - 72*ax*py_2*r2_2;
			const double a4 = 50*ax_4 + 100*ax_2*ay_2 + 50*ay_4 - 180*ax_2*ay*px - 180*ay_3*px + 162*ax_2*px_2 + 162*ay_2*px_2 + 180*ax_3*py + 180*ax*ay_2*py + 162*ax_2*py_2 + 162*ay_2*py_2;


//			std::cout << "a0 " << a0 << ", " << a1 << ", " << a2 << ", " << a3 << ", " << a4 << std::endl;
			std::vector<double> coefficients{a0, a1, a2, a3, a4};
			std::list<double> roots;
			billiards::shots::math::compute_roots(coefficients, roots);
			for (const double x : roots) {
//				std::cout << "x = " << x << std::endl;
				const double y_rad = std::pow(r1 + r2, 2) - std::pow(x, 2);
				if (y_rad < 0) {
					continue;
				}
				const double y = std::sqrt(y_rad);


				const double b0 = px_2 + py_2 - r1_2 - 2*px*x + std::pow(x, 2) - 2*py*y + std::pow(y, 2);
				const double b1 = -px_2 - py_2 + 2*px*x - std::pow(x, 2) + 2*py*y - std::pow(y, 2);
				const double b2 = 1/ (double) 2*px_2 + 1/ (double) 2*py_2 - px*x + 1/ (double) 2*std::pow(x, 2) - py*y + 1/ (double) 2*std::pow(y, 2);

				std::cout << "b " << b0 << ", " << b1 << ", " << b2 << std::endl;

				std::vector<double> u_coefficients{b0, b1, b2};
				std::list<double> u_roots;
				billiards::shots::math::compute_roots(u_coefficients, u_roots);
				for (const double u : u_roots) {
					std::cout << "x=" << x << ", y=" << y << ", u=" << u << std::endl;
				}
			}
		}

		{
			const double a0 = -49*ax_4*px_2*r1_2 - 70*ax_3*ay*px_2*r1_2 - 53*ax_2*ay_2*px_2*r1_2 - 20*ax*ay_3*px_2*r1_2 - 4*ay_4*px_2*r1_2 + 98*ax_4*px*py*r1_2 + 6*ax_2*ay_2*px*py*r1_2 + 8*ay_4*px*py*r1_2 - 49*ax_4*py_2*r1_2 + 70*ax_3*ay*py_2*r1_2 - 53*ax_2*ay_2*py_2*r1_2 + 20*ax*ay_3*py_2*r1_2 - 4*ay_4*py_2*r1_2 + 100*ax_2*ay_2*r1_4 - 140*ax_3*px*r1_4 + 40*ax*ay_2*px*r1_4 - 3*ax_2*px_2*r1_4 + 20*ax*ay*px_2*r1_4 + 8*ay_2*px_2*r1_4 + 140*ax_3*py*r1_4 - 40*ax*ay_2*py*r1_4 + 106*ax_2*px*py*r1_4 - 16*ay_2*px*py*r1_4 - 3*ax_2*py_2*r1_4 - 20*ax*ay*py_2*r1_4 + 8*ay_2*py_2*r1_4 - 100*ax_2*r1_6 - 40*ax*px*r1_6 - 4*px_2*r1_6 + 40*ax*py*r1_6 + 8*px*py*r1_6 - 4*py_2*r1_6 - 98*ax_4*px_2*r1*r2 - 140*ax_3*ay*px_2*r1*r2 - 106*ax_2*ay_2*px_2*r1*r2 - 40*ax*ay_3*px_2*r1*r2 - 8*ay_4*px_2*r1*r2 + 196*ax_4*px*py*r1*r2 + 12*ax_2*ay_2*px*py*r1*r2 + 16*ay_4*px*py*r1*r2 - 98*ax_4*py_2*r1*r2 + 140*ax_3*ay*py_2*r1*r2 - 106*ax_2*ay_2*py_2*r1*r2 + 40*ax*ay_3*py_2*r1*r2 - 8*ay_4*py_2*r1*r2 + 400*ax_2*ay_2*r1_3*r2 - 560*ax_3*px*r1_3*r2 + 160*ax*ay_2*px*r1_3*r2 - 12*ax_2*px_2*r1_3*r2 + 80*ax*ay*px_2*r1_3*r2 + 32*ay_2*px_2*r1_3*r2 + 560*ax_3*py*r1_3*r2 - 160*ax*ay_2*py*r1_3*r2 + 424*ax_2*px*py*r1_3*r2 - 64*ay_2*px*py*r1_3*r2 - 12*ax_2*py_2*r1_3*r2 - 80*ax*ay*py_2*r1_3*r2 + 32*ay_2*py_2*r1_3*r2 - 600*ax_2*r1_5*r2 - 240*ax*px*r1_5*r2 - 24*px_2*r1_5*r2 + 240*ax*py*r1_5*r2 + 48*px*py*r1_5*r2 - 24*py_2*r1_5*r2 - 49*ax_4*px_2*r2_2 - 70*ax_3*ay*px_2*r2_2 - 53*ax_2*ay_2*px_2*r2_2 - 20*ax*ay_3*px_2*r2_2 - 4*ay_4*px_2*r2_2 + 98*ax_4*px*py*r2_2 + 6*ax_2*ay_2*px*py*r2_2 + 8*ay_4*px*py*r2_2 - 49*ax_4*py_2*r2_2 + 70*ax_3*ay*py_2*r2_2 - 53*ax_2*ay_2*py_2*r2_2 + 20*ax*ay_3*py_2*r2_2 - 4*ay_4*py_2*r2_2 + 600*ax_2*ay_2*r1_2*r2_2 - 840*ax_3*px*r1_2*r2_2 + 240*ax*ay_2*px*r1_2*r2_2 - 18*ax_2*px_2*r1_2*r2_2 + 120*ax*ay*px_2*r1_2*r2_2 + 48*ay_2*px_2*r1_2*r2_2 + 840*ax_3*py*r1_2*r2_2 - 240*ax*ay_2*py*r1_2*r2_2 + 636*ax_2*px*py*r1_2*r2_2 - 96*ay_2*px*py*r1_2*r2_2 - 18*ax_2*py_2*r1_2*r2_2 - 120*ax*ay*py_2*r1_2*r2_2 + 48*ay_2*py_2*r1_2*r2_2 - 1500*ax_2*r1_4*r2_2 - 600*ax*px*r1_4*r2_2 - 60*px_2*r1_4*r2_2 + 600*ax*py*r1_4*r2_2 + 120*px*py*r1_4*r2_2 - 60*py_2*r1_4*r2_2 + 400*ax_2*ay_2*r1*r2_3 - 560*ax_3*px*r1*r2_3 + 160*ax*ay_2*px*r1*r2_3 - 12*ax_2*px_2*r1*r2_3 + 80*ax*ay*px_2*r1*r2_3 + 32*ay_2*px_2*r1*r2_3 + 560*ax_3*py*r1*r2_3 - 160*ax*ay_2*py*r1*r2_3 + 424*ax_2*px*py*r1*r2_3 - 64*ay_2*px*py*r1*r2_3 - 12*ax_2*py_2*r1*r2_3 - 80*ax*ay*py_2*r1*r2_3 + 32*ay_2*py_2*r1*r2_3 - 2000*ax_2*r1_3*r2_3 - 800*ax*px*r1_3*r2_3 - 80*px_2*r1_3*r2_3 + 800*ax*py*r1_3*r2_3 + 160*px*py*r1_3*r2_3 - 80*py_2*r1_3*r2_3 + 100*ax_2*ay_2*r2_4 - 140*ax_3*px*r2_4 + 40*ax*ay_2*px*r2_4 - 3*ax_2*px_2*r2_4 + 20*ax*ay*px_2*r2_4 + 8*ay_2*px_2*r2_4 + 140*ax_3*py*r2_4 - 40*ax*ay_2*py*r2_4 + 106*ax_2*px*py*r2_4 - 16*ay_2*px*py*r2_4 - 3*ax_2*py_2*r2_4 - 20*ax*ay*py_2*r2_4 + 8*ay_2*py_2*r2_4 - 1500*ax_2*r1_2*r2_4 - 600*ax*px*r1_2*r2_4 - 60*px_2*r1_2*r2_4 + 600*ax*py*r1_2*r2_4 + 120*px*py*r1_2*r2_4 - 60*py_2*r1_2*r2_4 - 600*ax_2*r1*r2_5 - 240*ax*px*r1*r2_5 - 24*px_2*r1*r2_5 + 240*ax*py*r1*r2_5 + 48*px*py*r1*r2_5 - 24*py_2*r1*r2_5 - 100*ax_2*r2_6 - 40*ax*px*r2_6 - 4*px_2*r2_6 + 40*ax*py*r2_6 + 8*px*py*r2_6 - 4*py_2*r2_6;
			const double a1 = 140*ax_4*px*r1_2 + 140*ax_3*ay*px*r1_2 + 40*ax*ay_3*px*r1_2 - 40*ay_4*px*r1_2 + 146*ax_3*px_2*r1_2 + 30*ax_2*ay*px_2*r1_2 + 56*ax*ay_2*px_2*r1_2 + 20*ay_3*px_2*r1_2 - 140*ax_4*py*r1_2 + 140*ax_3*ay*py*r1_2 + 40*ax*ay_3*py*r1_2 + 40*ay_4*py*r1_2 - 212*ax_3*px*py*r1_2 - 192*ax*ay_2*px*py*r1_2 + 146*ax_3*py_2*r1_2 - 30*ax_2*ay*py_2*r1_2 + 56*ax*ay_2*py_2*r1_2 - 20*ay_3*py_2*r1_2 + 200*ax_3*r1_4 + 220*ax_2*px*r1_4 - 40*ax*ay*px*r1_4 + 40*ay_2*px*r1_4 + 56*ax*px_2*r1_4 - 20*ay*px_2*r1_4 - 220*ax_2*py*r1_4 - 40*ax*ay*py*r1_4 - 40*ay_2*py*r1_4 - 32*ax*px*py*r1_4 + 56*ax*py_2*r1_4 + 20*ay*py_2*r1_4 + 280*ax_4*px*r1*r2 + 280*ax_3*ay*px*r1*r2 + 80*ax*ay_3*px*r1*r2 - 80*ay_4*px*r1*r2 + 292*ax_3*px_2*r1*r2 + 60*ax_2*ay*px_2*r1*r2 + 112*ax*ay_2*px_2*r1*r2 + 40*ay_3*px_2*r1*r2 - 280*ax_4*py*r1*r2 + 280*ax_3*ay*py*r1*r2 + 80*ax*ay_3*py*r1*r2 + 80*ay_4*py*r1*r2 - 424*ax_3*px*py*r1*r2 - 384*ax*ay_2*px*py*r1*r2 + 292*ax_3*py_2*r1*r2 - 60*ax_2*ay*py_2*r1*r2 + 112*ax*ay_2*py_2*r1*r2 - 40*ay_3*py_2*r1*r2 + 800*ax_3*r1_3*r2 + 880*ax_2*px*r1_3*r2 - 160*ax*ay*px*r1_3*r2 + 160*ay_2*px*r1_3*r2 + 224*ax*px_2*r1_3*r2 - 80*ay*px_2*r1_3*r2 - 880*ax_2*py*r1_3*r2 - 160*ax*ay*py*r1_3*r2 - 160*ay_2*py*r1_3*r2 - 128*ax*px*py*r1_3*r2 + 224*ax*py_2*r1_3*r2 + 80*ay*py_2*r1_3*r2 + 140*ax_4*px*r2_2 + 140*ax_3*ay*px*r2_2 + 40*ax*ay_3*px*r2_2 - 40*ay_4*px*r2_2 + 146*ax_3*px_2*r2_2 + 30*ax_2*ay*px_2*r2_2 + 56*ax*ay_2*px_2*r2_2 + 20*ay_3*px_2*r2_2 - 140*ax_4*py*r2_2 + 140*ax_3*ay*py*r2_2 + 40*ax*ay_3*py*r2_2 + 40*ay_4*py*r2_2 - 212*ax_3*px*py*r2_2 - 192*ax*ay_2*px*py*r2_2 + 146*ax_3*py_2*r2_2 - 30*ax_2*ay*py_2*r2_2 + 56*ax*ay_2*py_2*r2_2 - 20*ay_3*py_2*r2_2 + 1200*ax_3*r1_2*r2_2 + 1320*ax_2*px*r1_2*r2_2 - 240*ax*ay*px*r1_2*r2_2 + 240*ay_2*px*r1_2*r2_2 + 336*ax*px_2*r1_2*r2_2 - 120*ay*px_2*r1_2*r2_2 - 1320*ax_2*py*r1_2*r2_2 - 240*ax*ay*py*r1_2*r2_2 - 240*ay_2*py*r1_2*r2_2 - 192*ax*px*py*r1_2*r2_2 + 336*ax*py_2*r1_2*r2_2 + 120*ay*py_2*r1_2*r2_2 + 800*ax_3*r1*r2_3 + 880*ax_2*px*r1*r2_3 - 160*ax*ay*px*r1*r2_3 + 160*ay_2*px*r1*r2_3 + 224*ax*px_2*r1*r2_3 - 80*ay*px_2*r1*r2_3 - 880*ax_2*py*r1*r2_3 - 160*ax*ay*py*r1*r2_3 - 160*ay_2*py*r1*r2_3 - 128*ax*px*py*r1*r2_3 + 224*ax*py_2*r1*r2_3 + 80*ay*py_2*r1*r2_3 + 200*ax_3*r2_4 + 220*ax_2*px*r2_4 - 40*ax*ay*px*r2_4 + 40*ay_2*px*r2_4 + 56*ax*px_2*r2_4 - 20*ay*px_2*r2_4 - 220*ax_2*py*r2_4 - 40*ax*ay*py*r2_4 - 40*ay_2*py*r2_4 - 32*ax*px*py*r2_4 + 56*ax*py_2*r2_4 + 20*ay*py_2*r2_4;
			const double a2 = 53*ax_4*px_2 + 90*ax_3*ay*px_2 + 106*ax_2*ay_2*px_2 + 90*ax*ay_3*px_2 + 53*ay_4*px_2 - 90*ax_4*px*py + 90*ay_4*px*py + 53*ax_4*py_2 - 90*ax_3*ay*py_2 + 106*ax_2*ay_2*py_2 - 90*ax*ay_3*py_2 + 53*ay_4*py_2 - 100*ax_4*r1_2 - 200*ax_2*ay_2*r1_2 - 100*ay_4*r1_2 - 40*ax_3*px*r1_2 - 60*ax_2*ay*px*r1_2 - 20*ax*ay_2*px*r1_2 - 40*ay_3*px*r1_2 - 135*ax_2*px_2*r1_2 + 40*ax*ay*px_2*r1_2 - 117*ay_2*px_2*r1_2 + 40*ax_3*py*r1_2 - 60*ax_2*ay*py*r1_2 + 20*ax*ay_2*py*r1_2 - 40*ay_3*py*r1_2 - 58*ax_2*px*py*r1_2 + 22*ay_2*px*py*r1_2 - 135*ax_2*py_2*r1_2 - 40*ax*ay*py_2*r1_2 - 117*ay_2*py_2*r1_2 + 100*ax_2*r1_4 + 100*ay_2*r1_4 + 40*ax*px*r1_4 + 40*ay*px*r1_4 + 8*px_2*r1_4 - 40*ax*py*r1_4 + 40*ay*py*r1_4 + 8*py_2*r1_4 - 200*ax_4*r1*r2 - 400*ax_2*ay_2*r1*r2 - 200*ay_4*r1*r2 - 80*ax_3*px*r1*r2 - 120*ax_2*ay*px*r1*r2 - 40*ax*ay_2*px*r1*r2 - 80*ay_3*px*r1*r2 - 270*ax_2*px_2*r1*r2 + 80*ax*ay*px_2*r1*r2 - 234*ay_2*px_2*r1*r2 + 80*ax_3*py*r1*r2 - 120*ax_2*ay*py*r1*r2 + 40*ax*ay_2*py*r1*r2 - 80*ay_3*py*r1*r2 - 116*ax_2*px*py*r1*r2 + 44*ay_2*px*py*r1*r2 - 270*ax_2*py_2*r1*r2 - 80*ax*ay*py_2*r1*r2 - 234*ay_2*py_2*r1*r2 + 400*ax_2*r1_3*r2 + 400*ay_2*r1_3*r2 + 160*ax*px*r1_3*r2 + 160*ay*px*r1_3*r2 + 32*px_2*r1_3*r2 - 160*ax*py*r1_3*r2 + 160*ay*py*r1_3*r2 + 32*py_2*r1_3*r2 - 100*ax_4*r2_2 - 200*ax_2*ay_2*r2_2 - 100*ay_4*r2_2 - 40*ax_3*px*r2_2 - 60*ax_2*ay*px*r2_2 - 20*ax*ay_2*px*r2_2 - 40*ay_3*px*r2_2 - 135*ax_2*px_2*r2_2 + 40*ax*ay*px_2*r2_2 - 117*ay_2*px_2*r2_2 + 40*ax_3*py*r2_2 - 60*ax_2*ay*py*r2_2 + 20*ax*ay_2*py*r2_2 - 40*ay_3*py*r2_2 - 58*ax_2*px*py*r2_2 + 22*ay_2*px*py*r2_2 - 135*ax_2*py_2*r2_2 - 40*ax*ay*py_2*r2_2 - 117*ay_2*py_2*r2_2 + 600*ax_2*r1_2*r2_2 + 600*ay_2*r1_2*r2_2 + 240*ax*px*r1_2*r2_2 + 240*ay*px*r1_2*r2_2 + 48*px_2*r1_2*r2_2 - 240*ax*py*r1_2*r2_2 + 240*ay*py*r1_2*r2_2 + 48*py_2*r1_2*r2_2 + 400*ax_2*r1*r2_3 + 400*ay_2*r1*r2_3 + 160*ax*px*r1*r2_3 + 160*ay*px*r1*r2_3 + 32*px_2*r1*r2_3 - 160*ax*py*r1*r2_3 + 160*ay*py*r1*r2_3 + 32*py_2*r1*r2_3 + 100*ax_2*r2_4 + 100*ay_2*r2_4 + 40*ax*px*r2_4 + 40*ay*px*r2_4 + 8*px_2*r2_4 - 40*ax*py*r2_4 + 40*ay*py*r2_4 + 8*py_2*r2_4;
			const double a3 = -140*ax_4*px - 180*ax_3*ay*px - 100*ax_2*ay_2*px - 180*ax*ay_3*px + 40*ay_4*px - 162*ax_3*px_2 - 90*ax_2*ay*px_2 - 162*ax*ay_2*px_2 - 90*ay_3*px_2 + 140*ax_4*py - 180*ax_3*ay*py + 100*ax_2*ay_2*py - 180*ax*ay_3*py - 40*ay_4*py + 180*ax_3*px*py + 180*ax*ay_2*px*py - 162*ax_3*py_2 + 90*ax_2*ay*py_2 - 162*ax*ay_2*py_2 + 90*ay_3*py_2 - 200*ax_3*r1_2 - 200*ax*ay_2*r1_2 - 220*ax_2*px*r1_2 - 80*ax*ay*px*r1_2 - 140*ay_2*px*r1_2 - 72*ax*px_2*r1_2 + 220*ax_2*py*r1_2 - 80*ax*ay*py*r1_2 + 140*ay_2*py*r1_2 - 72*ax*py_2*r1_2 - 400*ax_3*r1*r2 - 400*ax*ay_2*r1*r2 - 440*ax_2*px*r1*r2 - 160*ax*ay*px*r1*r2 - 280*ay_2*px*r1*r2 - 144*ax*px_2*r1*r2 + 440*ax_2*py*r1*r2 - 160*ax*ay*py*r1*r2 + 280*ay_2*py*r1*r2 - 144*ax*py_2*r1*r2 - 200*ax_3*r2_2 - 200*ax*ay_2*r2_2 - 220*ax_2*px*r2_2 - 80*ax*ay*px*r2_2 - 140*ay_2*px*r2_2 - 72*ax*px_2*r2_2 + 220*ax_2*py*r2_2 - 80*ax*ay*py*r2_2 + 140*ay_2*py*r2_2 - 72*ax*py_2*r2_2;
			const double a4 = 100*ax_4 + 200*ax_2*ay_2 + 100*ay_4 + 180*ax_3*px + 180*ax_2*ay*px + 180*ax*ay_2*px + 180*ay_3*px + 162*ax_2*px_2 + 162*ay_2*px_2 - 180*ax_3*py + 180*ax_2*ay*py - 180*ax*ay_2*py + 180*ay_3*py + 162*ax_2*py_2 + 162*ay_2*py_2;

//			std::cout << "a0 " << a0 << ", " << a1 << ", " << a2 << ", " << a3 << ", " << a4 << std::endl;

			std::vector<double> coefficients{a0, a1, a2, a3, a4};
			std::list<double> roots;
			billiards::shots::math::compute_roots(coefficients, roots);
			for (const double x : roots) {
//				std::cout << "x = " << x << std::endl;
				const double y_rad = std::pow(r1 + r2, 2) - std::pow(x, 2);
				if (y_rad < 0) {
					continue;
				}
				const double y = std::sqrt(y_rad);

				const double b0 = px_2 + py_2 - r1_2 - 2*px*x + std::pow(x, 2) - 2*py*y + std::pow(y, 2);
				const double b1 = -px_2 - py_2 + px*x - py*x + px*y + py*y;
				const double b2 = 1/ (double) 2*px_2 + 1/ (double) 2*py_2 - px*x + py*x + std::pow(x, 2) - px*y - py*y + std::pow(y, 2);

				std::cout << "b " << b0 << ", " << b1 << ", " << b2 << std::endl;

				std::vector<double> u_coefficients{b0, b1, b2};
				std::list<double> u_roots;

				billiards::shots::math::compute_roots(u_coefficients, u_roots);
				for (const double u : u_roots) {
					std::cout << "x=" << x << ", y=" << y << ", u=" << u << std::endl;
				}
			}
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

#endif //IDEA_ROLLING_GLANCE_UNKNOWN_DEST_H
