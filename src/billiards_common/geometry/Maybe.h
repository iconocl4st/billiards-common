//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_MAYBE_H
#define IDEA_MAYBE_H

#include "billiards_common/geometry/Point.h"
#include "billiards_common/geometry/Line.h"

/*
 *
 * These classes were misguided.
 *
 * I should have created a way to ensure that I don't divide by zero,
 * rather than pass nothing along through calculations
 */

namespace billiards::geometry {

	namespace bv {
		enum bv {
			TRUE,
			FALSE,
			UNKNOWN,
		};
	}
	class MaybeBool {
	public:
		bv::bv v;

		MaybeBool(bv::bv v) : v{v} {};

		MaybeBool(bool valid, bool value) {
			if (valid) {
				if (value) {
					v = bv::TRUE;
				} else {
					v = bv::FALSE;
				}
			} else {
				v = bv::UNKNOWN;
			}
		}
		MaybeBool() : MaybeBool{false, false} {}

		~MaybeBool() = default;

		[[nodiscard]] inline
		bool is_valid() const {
			return v != bv::UNKNOWN;
		}


		[[nodiscard]] inline
		bool unsafe_get() const {
			switch (v) {
				case bv::TRUE:
					return true;
				default:
					return false;
			}
		}

		[[nodiscard]] inline
		bool get() const {
			switch (v) {
				case bv::FALSE:
					return false;
				case bv::TRUE:
					return true;
				case bv::UNKNOWN:
				default:
					throw std::runtime_error{"No value for bool"};
			}
		}

		[[nodiscard]] inline
		bool get_or(bool default_value) const {
			switch (v) {
				case bv::FALSE:
					return false;
				case bv::TRUE:
					return true;
				case bv::UNKNOWN:
				default:
					return default_value;
			}
		}

		[[nodiscard]] inline
		MaybeBool operator&&(const MaybeBool& other) const {
			return MaybeBool{is_valid() && other.is_valid(), unsafe_get() && other.unsafe_get()};
		};

		[[nodiscard]] inline
		MaybeBool operator||(const MaybeBool& other) const {
			return MaybeBool{is_valid() && other.is_valid(), unsafe_get() || other.unsafe_get()};
		};

		friend
		std::ostream& operator<<(std::ostream& os, const MaybeBool& p) {
			switch (p.v) {
				case bv::TRUE:
					os << "true";
					break;
				case bv::FALSE:
					os << "false";
					break;
				case bv::UNKNOWN:
				default:
					os << "<invalid>";
			}
			return os;
		}
	};


	namespace sgn {
		enum sgn {
			POSITIVE,
			NEGATIVE,
			ZERO,
			UNKNOWN,
		};
	}

	class MaybeSignum {
	public:
		sgn::sgn v;

		MaybeSignum(sgn::sgn value) : v{value} {}
		~MaybeSignum() = default;

		[[nodiscard]] inline
		MaybeBool operator==(const MaybeSignum& other) const {
			return MaybeBool{v != sgn::UNKNOWN && other.v != sgn::UNKNOWN, v == other.v};
		}
	};

	class MaybeDouble {
	public:
		bool valid;
		double value;

		MaybeDouble(bool valid, double value) : valid{valid}, value{value} {}
		MaybeDouble() : MaybeDouble{false, 0.0} {}
		MaybeDouble(double value) : MaybeDouble{!std::isnan(value) && !std::isinf(value), value} {}

		~MaybeDouble() = default;

		inline
		void operator=(const double v) {
			valid = !std::isnan(value) && !std::isinf(value);
			value = v;
		}

		[[nodiscard]] inline
		bool is_valid() const {
			return valid;
		}

		[[nodiscard]] inline
		double get() const {
			if (!is_valid()) {
				throw std::runtime_error{"No value for double"};
			}
			return value;
		}

		[[nodiscard]] inline
		double get_or(double default_value) const {
			if (!is_valid()) {
				return default_value;
			}
			return value;
		}

		[[nodiscard]] inline
		bool can_divide() const {
			return valid && std::abs(value) >= TOLERANCE;
		}

		[[nodiscard]] inline
		bool cannot_divide() const {
			return !valid || std::abs(value) < TOLERANCE;
		}

		inline
		MaybeDouble& operator+=(const MaybeDouble& other) {
			valid &= other.valid;
			value += other.value;
			return *this;
		}

		[[nodiscard]] inline
		MaybeDouble operator-() const {
			return MaybeDouble{valid, -value};
		}

		[[nodiscard]] inline
		MaybeBool operator<(MaybeDouble o) const {
			return MaybeBool{valid && o.valid, value < o.value};
		}

		[[nodiscard]] inline
		MaybeBool operator<=(MaybeDouble o) const {
			return MaybeBool{valid && o.valid, value <= o.value};
		}

		[[nodiscard]] inline
		MaybeBool operator>(MaybeDouble o) const {
			return MaybeBool{valid && o.valid, value > o.value};
		}

		[[nodiscard]] inline
		MaybeBool operator>=(MaybeDouble o) const {
			return MaybeBool{valid && o.valid, value >= o.value};
		}

		[[nodiscard]] inline
		MaybeBool operator<(double o) const {
			return MaybeBool{valid, value < o};
		}

		[[nodiscard]] inline
		MaybeBool operator<=(double o) const {
			return MaybeBool{valid, value <= o};
		}

		[[nodiscard]] inline
		MaybeBool operator>(double o) const {
			return MaybeBool{valid, value > o};
		}

		[[nodiscard]] inline
		MaybeBool operator>=(double o) const {
			return MaybeBool{valid, value >= o};
		}

		[[nodiscard]] inline
		double unsafe_get() const {
			return value;
		}

		[[nodiscard]] inline
		MaybeDouble operator-(const MaybeDouble& other) const {
			return MaybeDouble{valid && other.valid, value - other.value};
		};

		[[nodiscard]] inline
		MaybeDouble operator+(const MaybeDouble& other) const {
			return MaybeDouble{valid && other.valid, value + other.value};
		};

		[[nodiscard]] inline
		MaybeDouble operator*(const MaybeDouble& other) const {
			return MaybeDouble{valid && other.valid, value * other.value};
		};

		[[nodiscard]] inline
		MaybeDouble operator/(const MaybeDouble& other) const {
			if (valid && other.valid && std::abs(other.value) < TOLERANCE) {
				throw std::runtime_error("division by zero");
			}
			const bool v = valid && other.valid && std::abs(other.value) > TOLERANCE;
			return MaybeDouble{v, v ? (value / other.value) : NAN};
		};

		[[nodiscard]] inline
		MaybeDouble sqrt() const {
			if (valid && value < 0) {
				throw std::runtime_error("square root of negative");
			}
			const bool v = valid && value >= 0;
			return MaybeDouble{v, v ? std::sqrt(value) : NAN};
		}

		[[nodiscard]] inline
		MaybeDouble acos() const {
			return MaybeDouble{valid, std::acos(value)};
		}

		[[nodiscard]] inline
		MaybeDouble abs() const {
			return MaybeDouble{valid, std::abs(value)};
		}

		[[nodiscard]] inline
		MaybeDouble min(const MaybeDouble& other) const {
			return MaybeDouble{valid && other.valid, std::min(value, other.value)};
		}

		[[nodiscard]] inline
		MaybeDouble max(const MaybeDouble& other) const {
			return MaybeDouble{valid && other.valid, std::max(value, other.value)};
		}

		[[nodiscard]] inline
		MaybeSignum signum() const {
			if (!is_valid()) {
				return MaybeSignum{sgn::UNKNOWN};
			} else if (value > 0) {
				return MaybeSignum{sgn::POSITIVE};
			} else if (value < 0) {
				return MaybeSignum{sgn::NEGATIVE};
			} else {
				return MaybeSignum{sgn::ZERO};
			}
		}

		[[nodiscard]] inline
		MaybeBool in(double min, double max) const {
			return MaybeBool{is_valid(), min <= value && value <= max};
		}

		friend
		std::ostream& operator<<(std::ostream& os, const MaybeDouble& p) {
			if (p.valid) {
				os << p.value;
			} else {
				os << "<invalid>";
			}
			return os;
		}
	};

	class MaybePoint : public billiards::json::Serializable {
	public:
		MaybeDouble x;
		MaybeDouble y;

		MaybePoint(const Point& point) : x{point.x}, y{point.y} {}
		MaybePoint(const MaybeDouble x, const MaybeDouble& y) : x{x}, y{y} {}
		MaybePoint() : x{}, y{} {}

		~MaybePoint() = default;

		friend
		std::ostream& operator<<(std::ostream& os, const MaybePoint& p) {
			os << "{x:" << p.x << ",y:" << p.y << "}";
			return os;
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			if (HAS_NUMBER(value, "x")) {
				x = value["x"].get<double>();
			}
			if (HAS_NUMBER(value, "y")) {
				y = value["y"].get<double>();
			}
		}

		void to_json(billiards::json::SaxWriter& writer) const override {
			if (!is_valid()) {
				throw std::runtime_error{"Serializing an invalid point"};
			}
			writer.begin_object();
			writer.field("x", x.value);
			writer.field("y", y.value);
			writer.end_object();
		}

		inline
		void operator=(const Point& p) {
			x = p.x;
			y = p.y;
		}

		[[nodiscard]] inline
		bool is_valid() const {
			return x.is_valid() && y.is_valid();
		}

		[[nodiscard]] inline
		Point point() const {
			if (!is_valid()) {
				throw std::runtime_error{"No value for point"};
			}
			return Point{x.value, y.value};
		}

		[[nodiscard]] inline
		Point get() const {
			return point();
		}

		[[nodiscard]] inline
		MaybeDouble norm2() const {
			return x * x + y * y;
		}

		[[nodiscard]] inline
		MaybeDouble norm() const {
			return norm2().sqrt();
		}

		[[nodiscard]] inline
		MaybeDouble dot(const MaybePoint& other) const {
			return x * other.x + y * other.y;
		}

		[[nodiscard]] inline
		MaybePoint min(const MaybePoint& other) const {
			return MaybePoint{x.min(other.x), y.min(other.y)};
		}

		[[nodiscard]] inline
		MaybePoint max(const MaybePoint& other) const {
			return MaybePoint{x.max(other.x), y.max(other.y)};
		}

		[[nodiscard]] inline
		MaybePoint operator-(const MaybePoint& other) const {
			return MaybePoint{x - other.x, y - other.y};
		}

		[[nodiscard]] inline
		MaybePoint operator+(const MaybePoint& other) const {
			return MaybePoint{x + other.x, y + other.y};
		}

		inline
		MaybePoint& operator+=(const MaybePoint& other) {
			x += other.x;
			y += other.y;
			return *this;
		}

		[[nodiscard]] inline
		MaybePoint operator/(const MaybeDouble& val) const {
			return MaybePoint{x / val, y / val};
		}

		[[nodiscard]] inline
		MaybePoint operator*(const MaybeDouble& val) const {
			return MaybePoint{x * val, y * val};
		}
	};

	class MaybeLine {
	public:
		// Represents the set of points that satisfy a * x + b * y + c == 0
		MaybeDouble a, b, c;

		MaybeLine(MaybeDouble a, MaybeDouble b, MaybeDouble c) : a{a}, b{b}, c{c} {}

		~MaybeLine() = default;

		[[nodiscard]] inline
		MaybeLine normalize() const {
			MaybeDouble n = (a * a + b * b).sqrt();
			if (c.get_or(0) >= 0.0) {
				return MaybeLine{a / n, b / n, c / n};
			} else {
				return MaybeLine{-a / n, -b / n, -c / n};
			}
		}

		[[nodiscard]] inline
		MaybeDouble at(const MaybePoint& point) const {
			return MaybeDouble{a * point.x + b * point.y + c};
		}

		[[nodiscard]] inline
		Line get() const {
			return Line{a.get(), b.get(), c.get()};
		}
	};

	[[nodiscard]] inline
	MaybeDouble ternary(const MaybeBool& b, const MaybeDouble& d1, const MaybeDouble& d2) {
		return MaybeDouble{
			b.is_valid() && d1.is_valid() && d2.is_valid(),
			b.unsafe_get() ? d1.value : d2.value};
	}
}



#endif //IDEA_MAYBE_H
