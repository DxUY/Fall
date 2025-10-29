#pragma once

namespace Math {
	struct Vector2 {
		float X, Y;

		static Vector2 Zero() { return Vector2(0.0f); }

		Vector2(float scalar)
			: X(scalar), Y(scalar) {
		}

		Vector2(float x, float y)
			: X(x), Y(y) {
		}

		Vector2() : X(0.0f), Y(0.0f) {}

		friend Vector2 operator+(const Vector2& a, const Vector2& b) {
			return Vector2(a.X + b.X, a.Y + b.Y);
		}

		friend Vector2 operator*(const Vector2& v, float scalar) {
			return Vector2(v.X * scalar, v.Y * scalar);
		}

		friend Vector2 operator*(float scalar, const Vector2& v) {
			return Vector2(v.X * scalar, v.Y * scalar);
		}

		float LengthSquared() const {
			return X * X + Y * Y;
		}

		float Length() const {
			return std::sqrt(LengthSquared());
		}
	};
}