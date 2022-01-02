//
// Created by thallock on 10/3/21.
//

#ifndef IDEA_ROLLING_GLANCE_H
#define IDEA_ROLLING_GLANCE_H

//#include "billiards_common/math/polynomial.h"
#include "billiards_common/shots/RollingGlanceCalculation.h"
#include "billiards_common/shots/shot_calculation/math/high_order_polys.h"

namespace billiards::shots::math {


	inline
	void get_glance_location(
		const double ax, const double ay,
		const double dx, const double dy,
		const double r,
		const std::function<void(const billiards::shots::RollingGlanceCalculation&)>& receiver
	) {
		const double dx_2 = dx * dx;
		const double dy_2 = dy * dy;
		const double ax_2 = ax * ax;
		const double ax_3 = ax_2 * ax;
		const double ax_4 = ax_3 * ax;
		const double ay_2 = ay * ay;
		const double ay_3 = ay_2 * ay;
		const double ay_4 = ay_3 * ay;
		const double r_2 = r * r;
		const double r_3 = r_2 * r;
		const double r_4 = r_3 * r;
		const double r_5 = r_4 * r;
		const double r_6 = r_5 * r;

//		const double na2 = ax * ax + ay * ay;
//		if (std::abs(na2) < TOLERANCE) {
//			// cue is distance 0 away...
//			return;
//		}
//		if (std::abs(dx) < TOLERANCE && std::abs(dy) < TOLERANCE) {
//			// cue is at destination...
//			return;
//		}

		const auto checker = [&](const RollingGlanceCalculation& rgc) {
			const double x = rgc.loc.x;
			const double y = rgc.loc.y;
			const double tx = rgc.tan_dir.x;
			const double ty = rgc.tan_dir.y;
			const double aim_x = rgc.aim_dir.x;
			const double aim_y = rgc.aim_dir.y;

			const double alpha = 2 / 7.0;
			double t;
			if (std::abs(dx - x) >= TOLERANCE) {
				t = (alpha * (tx - x) + (1 - alpha) * (aim_x - x)) / (dx - x);
			} else if (std::abs(dy - y) >= TOLERANCE) {
				t = (alpha * (ty - y) + (1 - alpha) * (aim_y - y)) / (dy - y);
			} else {
				throw std::runtime_error{"We already checked whether dx and dy are zero..."};
			}

			const double orthogonality_at_s = (rgc.aim_dir.x - x) * (rgc.aim_dir.x - tx) + (rgc.aim_dir.y - y) * (rgc.aim_dir.y - ty);
			std::cout << "orthogonality: " << orthogonality_at_s << std::endl;
			if (std::abs(orthogonality_at_s) > CHECK_TOL) {
				return;
			}

			const double rolling_x = alpha * (tx - x) + (1 - alpha) * (aim_x - x) - t * (dx - x);
			std::cout << "rolling_x: " << rolling_x << std::endl;
			if (std::abs(rolling_x) > CHECK_TOL) {
				return;
			}
			const double rolling_y = alpha * (ty - y) + (1 - alpha) * (aim_y - y) - t * (dy - y);
			std::cout << "rolling_y: " << rolling_y << std::endl;
			if (std::abs(rolling_y) > CHECK_TOL) {
				return;
			}

			const double t_same_direction = (tx - x) * (dx - x) + (ty - y) * (dy - y);
			std::cout << "t same direction: " << t_same_direction << std::endl;
			if (t_same_direction < 0) {
				return;
			}
			const double d_same_direction = (tx - x) * (dx - x) + (ty - y) * (dy - y);
			std::cout << "d same direction as t: " << d_same_direction << std::endl;
			if (d_same_direction < 0) {
				return;
			}
			const double a_same_direction = x * (ax - x) + y * (ay - y);
			std::cout << "a same direction: " << a_same_direction << std::endl;
			if (a_same_direction < 0) {
				return;
			}
			receiver(rgc);
		};

		{
			const double a0 = -25/ (double) 49*ax_2*ay_2*dx_2*r_2 + 10/ (double) 7*ax_3*ay*dx*dy*r_2 + 20/ (double) 49*ax*ay_3*dx*dy*r_2 - ax_4*dy_2*r_2 - 4/ (double) 7*ax_2*ay_2*dy_2*r_2 - 4/ (double) 49*ay_4*dy_2*r_2 + ax_4*r_4 + 4/ (double) 7*ax_2*ay_2*r_4 + 4/ (double) 49*ay_4*r_4 - 10/ (double) 7*ax_3*dx*r_4 + 20/ (double) 49*ax*ay_2*dx*r_4 + 25/ (double) 49*ax_2*dx_2*r_4 - 20/ (double) 49*ax*ay*dx*dy*r_4 - 4/ (double) 7*ax_2*dy_2*r_4 + 8/ (double) 49*ay_2*dy_2*r_4 + 4/ (double) 7*ax_2*r_6 - 8/ (double) 49*ay_2*r_6 - 20/ (double) 49*ax*dx*r_6 - 4/ (double) 49*dy_2*r_6 + 4/ (double) 49*std::pow(r, 8);
			const double a1 = -4/ (double) 7*ax_4*dx*r_2 - 6/ (double) 49*ax_2*ay_2*dx*r_2 - 4/ (double) 7*ay_4*dx*r_2 + 20/ (double) 49*ax_3*dx_2*r_2 - 20/ (double) 49*ax*ay_2*dx_2*r_2 - 10/ (double) 7*ax_3*ay*dy*r_2 - 20/ (double) 49*ax*ay_3*dy*r_2 - 30/ (double) 49*ax_2*ay*dx*dy*r_2 - 20/ (double) 49*ay_3*dx*dy*r_2 + 18/ (double) 7*ax_3*dy_2*r_2 + 76/ (double) 49*ax*ay_2*dy_2*r_2 - 8/ (double) 7*ax_3*r_4 - 96/ (double) 49*ax*ay_2*r_4 + 4/ (double) 49*ax_2*dx*r_4 + 36/ (double) 49*ay_2*dx*r_4 + 20/ (double) 49*ax*dx_2*r_4 + 20/ (double) 49*ax*ay*dy*r_4 + 20/ (double) 49*ay*dx*dy*r_4 + 36/ (double) 49*ax*dy_2*r_4 - 16/ (double) 49*ax*r_6 - 8/ (double) 49*dx*r_6;
			const double a2 = 4/ (double) 49*ax_4*dx_2 + 53/ (double) 49*ax_2*ay_2*dx_2 + ay_4*dx_2 - 90/ (double) 49*ax_3*ay*dx*dy - 90/ (double) 49*ax*ay_3*dx*dy + ax_4*dy_2 + 53/ (double) 49*ax_2*ay_2*dy_2 + 4/ (double) 49*ay_4*dy_2 - 10/ (double) 7*ax_4*r_2 - 50/ (double) 49*ax_2*ay_2*r_2 + 20/ (double) 49*ay_4*r_2 + 192/ (double) 49*ax_3*dx*r_2 + 26/ (double) 7*ax*ay_2*dx*r_2 - 82/ (double) 49*ax_2*dx_2*r_2 - 53/ (double) 49*ay_2*dx_2*r_2 + 30/ (double) 49*ax_2*ay*dy*r_2 + 20/ (double) 49*ay_3*dy*r_2 - 40/ (double) 49*ax*ay*dx*dy*r_2 - 53/ (double) 49*ax_2*dy_2*r_2 - 64/ (double) 49*ay_2*dy_2*r_2 - 4/ (double) 49*ax_2*r_4 + 36/ (double) 49*ay_2*r_4 + 52/ (double) 49*ax*dx*r_4 + 4/ (double) 49*dx_2*r_4 - 20/ (double) 49*ay*dy*r_4 + 4/ (double) 49*dy_2*r_4;
			const double a3 = 20/ (double) 49*ax_4*dx - 50/ (double) 49*ax_2*ay_2*dx - 10/ (double) 7*ay_4*dx - 36/ (double) 49*ax_3*dx_2 - 36/ (double) 49*ax*ay_2*dx_2 + 90/ (double) 49*ax_3*ay*dy + 90/ (double) 49*ax*ay_3*dy + 90/ (double) 49*ax_2*ay*dx*dy + 90/ (double) 49*ay_3*dx*dy - 18/ (double) 7*ax_3*dy_2 - 18/ (double) 7*ax*ay_2*dy_2 + 40/ (double) 49*ax_3*r_2 + 40/ (double) 49*ax*ay_2*r_2 - 52/ (double) 49*ax_2*dx*r_2 - 92/ (double) 49*ay_2*dx*r_2 - 36/ (double) 49*ax*dx_2*r_2 + 40/ (double) 49*ax*ay*dy*r_2 - 36/ (double) 49*ax*dy_2*r_2;
			const double a4 = 25/ (double) 49*ax_4 + 50/ (double) 49*ax_2*ay_2 + 25/ (double) 49*ay_4 - 90/ (double) 49*ax_3*dx - 90/ (double) 49*ax*ay_2*dx + 81/ (double) 49*ax_2*dx_2 + 81/ (double) 49*ay_2*dx_2 - 90/ (double) 49*ax_2*ay*dy - 90/ (double) 49*ay_3*dy + 81/ (double) 49*ax_2*dy_2 + 81/ (double) 49*ay_2*dy_2;
			std::vector<double> coefficients{a0, a1, a2, a3, a4};

			std::list<double> roots;
			billiards::shots::math::compute_roots(coefficients, roots);
			for (const double x : roots) {
				const double rad = r * r - x * x;
				double y;
				if (rad > TOLERANCE) {
					y = std::sqrt(rad);
				} else if (rad > -TOLERANCE) {
					y = 0;
				} else {
					return;
				}
				const double tx = x + y;
				const double ty = -x + y;
				const double s_numerator = ay*x - ax*y;
				const double s_denominator = ax_2 + ay_2 - 2*ax*x + std::pow(x, 2) - 2*ay*y + std::pow(y, 2);
				if (std::abs(s_denominator) < TOLERANCE) {
					return;
				}
				const double s = s_numerator / s_denominator;
				const double aim_x = -(ax - x)*s + x;
				const double aim_y = -(ay - y)*s + y;
				checker(RollingGlanceCalculation{x, y, tx, ty, aim_x, aim_y});
			};
		}

		{
			const double a0 = -25/ (double) 49*ax_2*ay_2*dx_2*r_2 + 10/ (double) 7*ax_3*ay*dx*dy*r_2 + 20/ (double) 49*ax*ay_3*dx*dy*r_2 - ax_4*dy_2*r_2 - 4/ (double) 7*ax_2*ay_2*dy_2*r_2 - 4/ (double) 49*ay_4*dy_2*r_2 + ax_4*r_4 + 4/ (double) 7*ax_2*ay_2*r_4 + 4/ (double) 49*ay_4*r_4 - 10/ (double) 7*ax_3*dx*r_4 + 20/ (double) 49*ax*ay_2*dx*r_4 + 25/ (double) 49*ax_2*dx_2*r_4 - 20/ (double) 49*ax*ay*dx*dy*r_4 - 4/ (double) 7*ax_2*dy_2*r_4 + 8/ (double) 49*ay_2*dy_2*r_4 + 4/ (double) 7*ax_2*r_6 - 8/ (double) 49*ay_2*r_6 - 20/ (double) 49*ax*dx*r_6 - 4/ (double) 49*dy_2*r_6 + 4/ (double) 49*std::pow(r, 8);
			const double a1 = -4/ (double) 7*ax_4*dx*r_2 - 6/ (double) 49*ax_2*ay_2*dx*r_2 - 4/ (double) 7*ay_4*dx*r_2 + 20/ (double) 49*ax_3*dx_2*r_2 - 20/ (double) 49*ax*ay_2*dx_2*r_2 - 10/ (double) 7*ax_3*ay*dy*r_2 - 20/ (double) 49*ax*ay_3*dy*r_2 - 30/ (double) 49*ax_2*ay*dx*dy*r_2 - 20/ (double) 49*ay_3*dx*dy*r_2 + 18/ (double) 7*ax_3*dy_2*r_2 + 76/ (double) 49*ax*ay_2*dy_2*r_2 - 8/ (double) 7*ax_3*r_4 - 96/ (double) 49*ax*ay_2*r_4 + 4/ (double) 49*ax_2*dx*r_4 + 36/ (double) 49*ay_2*dx*r_4 + 20/ (double) 49*ax*dx_2*r_4 + 20/ (double) 49*ax*ay*dy*r_4 + 20/ (double) 49*ay*dx*dy*r_4 + 36/ (double) 49*ax*dy_2*r_4 - 16/ (double) 49*ax*r_6 - 8/ (double) 49*dx*r_6;
			const double a2 = 4/ (double) 49*ax_4*dx_2 + 53/ (double) 49*ax_2*ay_2*dx_2 + ay_4*dx_2 - 90/ (double) 49*ax_3*ay*dx*dy - 90/ (double) 49*ax*ay_3*dx*dy + ax_4*dy_2 + 53/ (double) 49*ax_2*ay_2*dy_2 + 4/ (double) 49*ay_4*dy_2 - 10/ (double) 7*ax_4*r_2 - 50/ (double) 49*ax_2*ay_2*r_2 + 20/ (double) 49*ay_4*r_2 + 192/ (double) 49*ax_3*dx*r_2 + 26/ (double) 7*ax*ay_2*dx*r_2 - 82/ (double) 49*ax_2*dx_2*r_2 - 53/ (double) 49*ay_2*dx_2*r_2 + 30/ (double) 49*ax_2*ay*dy*r_2 + 20/ (double) 49*ay_3*dy*r_2 - 40/ (double) 49*ax*ay*dx*dy*r_2 - 53/ (double) 49*ax_2*dy_2*r_2 - 64/ (double) 49*ay_2*dy_2*r_2 - 4/ (double) 49*ax_2*r_4 + 36/ (double) 49*ay_2*r_4 + 52/ (double) 49*ax*dx*r_4 + 4/ (double) 49*dx_2*r_4 - 20/ (double) 49*ay*dy*r_4 + 4/ (double) 49*dy_2*r_4;
			const double a3 = 20/ (double) 49*ax_4*dx - 50/ (double) 49*ax_2*ay_2*dx - 10/ (double) 7*ay_4*dx - 36/ (double) 49*ax_3*dx_2 - 36/ (double) 49*ax*ay_2*dx_2 + 90/ (double) 49*ax_3*ay*dy + 90/ (double) 49*ax*ay_3*dy + 90/ (double) 49*ax_2*ay*dx*dy + 90/ (double) 49*ay_3*dx*dy - 18/ (double) 7*ax_3*dy_2 - 18/ (double) 7*ax*ay_2*dy_2 + 40/ (double) 49*ax_3*r_2 + 40/ (double) 49*ax*ay_2*r_2 - 52/ (double) 49*ax_2*dx*r_2 - 92/ (double) 49*ay_2*dx*r_2 - 36/ (double) 49*ax*dx_2*r_2 + 40/ (double) 49*ax*ay*dy*r_2 - 36/ (double) 49*ax*dy_2*r_2;
			const double a4 = 25/ (double) 49*ax_4 + 50/ (double) 49*ax_2*ay_2 + 25/ (double) 49*ay_4 - 90/ (double) 49*ax_3*dx - 90/ (double) 49*ax*ay_2*dx + 81/ (double) 49*ax_2*dx_2 + 81/ (double) 49*ay_2*dx_2 - 90/ (double) 49*ax_2*ay*dy - 90/ (double) 49*ay_3*dy + 81/ (double) 49*ax_2*dy_2 + 81/ (double) 49*ay_2*dy_2;
			std::vector<double> coefficients{a0, a1, a2, a3, a4};

			std::list<double> roots;
			billiards::shots::math::compute_roots(coefficients, roots);
			for (const double x : roots) {
				const double rad = r * r - x * x;
				double y;
				if (rad > TOLERANCE) {
					y = std::sqrt(rad);
				} else if (rad > -TOLERANCE) {
					y = 0;
				} else {
					return;
				}
				const double tx = x - y;
				const double ty = x + y;
				const double s_numerator = -ay*x + ax*y;
				const double s_denominator = ax_2 + ay_2 - 2*ax*x + std::pow(x, 2) - 2*ay*y + std::pow(y, 2);
				if (std::abs(s_denominator) < TOLERANCE) {
					return;
				}
				const double s = s_numerator / s_denominator;
				const double aim_x = -(ax - x)*s + x;
				const double aim_y = -(ay - y)*s + y;
				checker(RollingGlanceCalculation{x, y, tx, ty, aim_x, aim_y});
			};
		}
	}


	inline
	void get_glance_location(
		const geometry::MaybePoint& cue_loc,
		const geometry::MaybePoint& destination_loc,
		const double r,
		const std::function<void(const billiards::shots::RollingGlanceCalculation&)>& receiver
	) {
		if (!cue_loc.is_valid() || !destination_loc.is_valid()) {
			return;
		}
		get_glance_location(
			cue_loc.x.get(), cue_loc.y.get(),
			destination_loc.x.get(), destination_loc.y.get(),
			r,
			receiver
		);
	}


#if 0
	void old_code() {

		const double a2 = a * a;
		const double b2 = b * b;
		const double r2 = r * r;

		a * ((p0 - c0) * b - p1 + c1)
		+ 1 / r ** 2 * (1 - a) * (
			p0 ** 2 * (c0 - p0) * b - p1 ** 2 * (c1 - p1) + p0 * p1 * ((c1 - p1) * b - (c0 - p0))
		)
	}
#endif



/*
 * Sage code no longer needed...


eq_r
poly = eq_06.left()



for ass, tan_sol in enumerate([{'tx': y, 'ty': -x}, {'tx': -y, 'ty': x}]):
	print('\t// tangent assignment ' + str(ass))
	print('\t// ' + str(poly.substitute(**tan_sol)))
	solve(eq_06.substitute(**tan_sol), y)
	for name, coeff in zip(
		['c00', 'c10', 'c01', 'c20', 'c11', 'c02'],
		get_coefficients(poly.substitute(**tan_sol).expand())
	):
		print('\tconst double ' + name + ' = ' + convert_powers(coeff).replace('(std::pow(ax, 2) + std::pow(ay, 2))', 'na2'))


eq_11 = eq_dy / eq_dx
eq_12 = eq_11 * ((alpha - 1)*ax*s - alpha*tx)
eq_13 = eq_12 - ( (alpha - 1)*ay*s - alpha*ty )
eq_14 = eq_13.substitute(s=s_sol)
eq_15 = eq_14.substitute(alpha=2/7)
poly1 = eq_15.left()

for denom, poly in [('dy', poly0), ('dx', poly1)]:
	print('// ' + denum + ' cannot be 0')
	for ass, tan_sol in enumerate([{'tx': y, 'ty': -x}, {'tx': -y, 'ty': x}]):
		print('// tangent assignment ' + str(ass))
		for name, coeff in zip(
			['c00', 'c10', 'c01', 'c20', 'c11', 'c02'],
			get_coefficients(poly.substitute(**tan_sol).expand())
		):
			print('\tconst double ' + name + ' = ' + convert_powers(coeff).replace('(std::pow(ax, 2) + std::pow(ay, 2))', 'na2'))






#R = PolynomialRing(QQ, names='x, y')
# assume(ax, ay, dx, dy, r, 'real')

(eq_dx / eq_dy * ((alpha - 1)*ay*s - alpha*ty)).substitute(s=s_sol)
(eq_dx / eq_dy * ((alpha - 1)*ay*s - alpha*ty) - (ax*tx + ay*ty)*(alpha - 1)*ax/(ax^2 + ay^2) - alpha*tx).substitute(s=s_sol).simplify()


	// dir == 2.0 / 7 * tan + 5.0 / 7 * t * aim
	// (t * aim - tan) @ tan == 0
	// t == 1 / (aim @ tan)

	// dir == 2.0 / 7 * tan + 5.0 / 7 * aim / (aim @ tan)
	// dir == 2.0 / 7 * tan + 5.0 / 7 * aim / (aim @ tan)

	// x, y
	// aim = {src.y - y, x - src.x, y * (src.x - x) + x * (y - src.y)} // runs through x,y
	// tan = {x - src.x, y - src.y, x * (src.x - x) + y * (src.y - y)} // runs though x, y; orthogonal to aim


	// d = 2/7 * n + 5/7 * t * a
	// (t * a - n) @ n == 0
	// t * a @ n == n @ n


	// a = p - src
	// n @ a == 0
	// n @ [p; 1] == 0
	// d = 5/7 * n @ n + 1 / (n @ a) * a + 2/7 * n
	// d = dst - p

	// nx * (x - sx) + ny * (y - sy) == 0
	// dst.x - p == 2/7 * nx + 5/7 * (nx ** 2 + ny ** 2 +  1 / (nx * (x - sx) + ny * (y - sy)) *



.replace('(std::pow(ax, 2) + std::pow(ay, 2))', 'na2')


 */
}






#endif //IDEA_ROLLING_GLANCE_H
