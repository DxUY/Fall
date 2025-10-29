#pragma once

#include "Vector2.h"

namespace Math {

	struct Vector3 {
		float X, Y, Z;

		Vector3()
			: X(0.0f), Y(0.0f), Z(0.0f) {
		}

		Vector3(float scalar)
			: X(scalar), Y(scalar), Z(scalar) {
		}

		Vector3(float x, float y, float z)
			: X(x), Y(y), Z(z) {
		}

		Vector3(const Vector2& xy, float z)
			: X(xy.X), Y(xy.Y), Z(z) {
		}

		Vector2 GetXY() const {
			return Vector2(X, Y);
		}

		void SetXY(const Vector2& value) {
			X = value.X;
			Y = value.Y;
		}

		static Vector3 Add(const Vector3& a, const Vector3& b) {
			return Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
		}

		static Vector3 Multiply(const Vector3& v, float scalar) {
			return Vector3(v.X * scalar, v.Y * scalar, v.Z * scalar);
		}

		Vector3 operator+(const Vector3& b) const {
			return Add(*this, b);
		}

		Vector3 operator*(float scalar) const {
			return Multiply(*this, scalar);
		}

		float LengthSquared() const {
			return X * X + Y * Y + Z * Z;
		}

		float Length() const {
			return std::sqrt(LengthSquared());
		}
	};

}