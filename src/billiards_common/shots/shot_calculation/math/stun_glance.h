//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_STUN_GLANCE_H
#define IDEA_STUN_GLANCE_H

namespace billiards::shots::math {

	[[nodiscard]] inline
	bool solve_edge_system(
		const MaybePoint& w, const MaybeDouble& r,
		MaybePoint& sol0, MaybePoint& sol1
	) {
		const auto r2 = r * r;
		const auto wx2 = w.x * w.x;
		const auto wy2 = w.y * w.y;

		const auto wx2pwy2 = wx2 + wy2;
		const auto rad = (wx2pwy2 - r2).sqrt();
		const auto x_den = wx2pwy2;
		const auto xp = w.x * r2;
		const auto y_den = w.y * wx2pwy2;
		const auto yp = wy2 * r2;

		const auto y_is_small = y_den.cannot_divide()
			|| w.y.cannot_divide()
			|| wy2.cannot_divide();
		const auto x_is_small = x_den.cannot_divide()
			|| w.x.cannot_divide()
			|| wx2.cannot_divide();

		if (y_is_small) {
			if (x_is_small) {
				// Infinitely man solutions if r == 0
				// No solutions otherwise
				return false;
			}

			sol0.x = r2 / w.x;
			sol0.y = (r2 - (-r2 / wx2 + 1)).sqrt();
			sol1.x = sol0.x;
			sol1.y = -sol0.y;
			return true;
		}
		if (x_is_small) {
			sol0.x = (r2 - (-r2 / wy2 + 1)).sqrt();
			sol0.y = r2 / w.y;
			sol1.x = -sol0.x;
			sol1.y = sol0.y;
			return true;
		}

		sol0.x = (xp -       w.y * r * rad) / x_den;
		sol0.y = (yp + w.x * w.y * r * rad) / y_den;
		sol1.x = (xp +       w.y * r * rad) / x_den;
		sol1.y = (yp - w.x * w.y * r * rad) / y_den;
		return true;
	}


	// Solves:
	// norm(gp - x) = radius
	// (x - obj) @ (x - gp) == 0
	[[nodiscard]] inline
	MaybePoint get_edge_point(
		const MaybePoint& obj_location, const MaybePoint& goal_post, const MaybeDouble radius,
		const MaybePoint& inner
	) {
		const auto line = through(obj_location, goal_post);
		const auto orientation = line.at(inner).signum();
		MaybePoint trial1; MaybePoint trial2;
		bool success = solve_edge_system(obj_location - goal_post, radius, trial1, trial2);
		if (!success) {
			// TODO: What do we do in this case...
			throw std::runtime_error{"Unhandled case..."};
		}
		{
			const auto ret = goal_post + trial1;
			if ((line.at(ret).signum() == orientation).get_or(false)) {
				return ret;
			}
		}
		{
			const auto ret = goal_post + trial2;
			if ((line.at(ret).signum() == orientation).get_or(false)) {
				return ret;
			}
		}
		return MaybePoint{};
	}
	[[nodiscard]] inline
	MaybePoint calculate_stun_glance(
		const MaybePoint& src,
		const MaybeDouble& src_r,
		const MaybePoint& obj,
		const MaybeDouble& obj_r,
		const MaybePoint& dst
	) {
		// Solve:
		// (x - obj.x) ** 2 + (y - obj.y) ** 2 == (obj_r + src_r) ** 2
		// (x - dst.x) * (x - src.x) + (y - dst.y) * (y - src.y) == 0
		return get_edge_point(src, obj, src_r + obj_r, dst);
	}

}
#endif //IDEA_STUN_GLANCE_H
