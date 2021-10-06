//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_LINEAR_SOLVE_H
#define IDEA_LINEAR_SOLVE_H

#include "billiards_common/geometry/Maybe.h"

namespace billiards::shots::linear {

	// TODO:
	// provide a default_x and default_y for infinite solutions...
// Could break this apart if we need to test if a triangle contains multiple points...
	class LinearSystem {
	public:
		double a[3 * 4];
		int pivots[3];

		LinearSystem(
			double a00, double a01, double a02, double b0,
			double a10, double a11, double a12, double b1,
			double a20, double a21, double a22, double b2
		) : a{}, pivots{0, 1, 2} {
			at(0, 0) = a00; at(0, 1) = a01; at(0, 2) = a02; at(0, 3) = b0;
			at(1, 0) = a10; at(1, 1) = a11; at(1, 2) = a12; at(1, 3) = b1;
			at(2, 0) = a20; at(2, 1) = a21; at(2, 2) = a22; at(2, 3) = b2;
		}

		~LinearSystem() = default;

		inline
		double& at(int r, int c) {
			return a[pivots[r] * 4 + c];
		}

		// TODO:
		inline
		void swap_rows(int r1, int r2) {
			if (r1 == r2) {
				return;
			}
			int tmp = pivots[r1];
			pivots[r1] = pivots[r2];
			pivots[r2] = tmp;
		}

		// Several of these are going to be zero...
		inline
		void divide_row(int r, double val) {
			for (int i = 0; i < 4; i++) {
				at(r, i) /= val;
			}
		}

		// Several of these are going to be zero...
		inline
		void row_op(int dest, double factor, int source) {
			for (int i = 0; i < 4; i++) {
				at(dest, i) += factor * at(source, i);
			}
		}

		inline
		void print() {
			std::cout << "View:" << std::endl;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << at(i, j) << " ";
				}
				std::cout << '\n';
			}
			std::cout << "Raw" << std::endl;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 4; j++) {
					std::cout << a[i * 4 + j] << " ";
				}
				std::cout << '\n';
			}
			std::cout << "Pivots:" << std::endl;
			print_vec(pivots);
			std::cout << std::endl;
		}
	};


	inline
	int get_pivot_row(LinearSystem& system, int col) {
		int index = col;
		double val = system.at(col, col);
		for (int i = col + 1; i < 3; i++) {
			double oval = std::abs(system.at(i, col));
			if (oval > val) {
				index = i;
				val = oval;
			}
		}
		return index;
	}


// only uses partial pivoting...
// could change this to householder for the fun of it...
	inline
	bool lu(LinearSystem& system) {
		for (int r = 0; r < 3; r++) {
			int pivot_row = get_pivot_row(system, r);
			system.swap_rows(r, pivot_row);
			double denom = system.at(r, r);
			if (std::abs(denom) < TOL) {
				return false;
			}
			system.divide_row(r, denom);
			for (int r2 = r + 1; r2 < 3; r2++) {
				system.row_op(r2, -system.at(r2, r), r);
			}
		}
		return true;
	}


	inline
	void substitute(LinearSystem& system, double *x) {
		x[2] = system.at(2, 3);
		x[1] = system.at(1, 3) - system.at(1, 2) * x[2];
		x[0] = system.at(0, 3) - system.at(0, 2) * x[2] - system.at(0, 1) * x[1];
	}

	inline
	bool in01(double x) {
		return 0 <= x && x <= 1;
	}

	inline
	bool in01(double *x) {
		return in01(x[0]) && in01(x[1]) && in01(x[2]);
	}

	inline
	double get_solution_error(
		double a00, double a01, double a02, double b0,
		double a10, double a11, double a12, double b1,
		double a20, double a21, double a22, double b2,
		double *x
	) {
		double e0 = a00 * x[0] + a01 * x[1] + a02 * x[2] - b0;
		double e1 = a10 * x[0] + a11 * x[1] + a12 * x[2] - b1;
		double e2 = a20 * x[0] + a21 * x[1] + a22 * x[2] - b2;
		double n2 = e0 * e0 + e1 * e1 + e2 * e2;
		return std::sqrt(n2);
	}

	inline
	MaybeBool triangle_contains(
		const MaybePoint& v1, const MaybePoint& v2, const MaybePoint& v3,
		const MaybePoint& p
	) {
		if (v1.is_valid() || !v2.is_valid() || !v3.is_valid() || !MaybePoint.is_valid()) {
			return MaybeBool{};
		}
		LinearSystem system{
			v1.x.get(), v2.x.get(), v3.x.get(), p.x.get(),
			v1.y.get(), v2.y.get(), v3.y.get(), p.y.get(),
			1, 1, 1, 1
		};

		bool success = lu(system);
		if (!success) {
			return MaybeBool{};
		}

		double x[3];
		substitute(system, x);

		double error = get_solution_error(
			v1.x.get(), v2.x.get(), v3.x.get(), p.x.get(),
			v1.y.get(), v2.y.get(), v3.y.get(), p.y.get(),
			1, 1, 1, 1,
			x
		);
		if (error > TOL) {
			return MaybeBool{};
		}

		return MaybeBool{true, in01(x)};
	}
}
#endif //IDEA_LINEAR_SOLVE_H
