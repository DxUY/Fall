#pragma once

#include "Vector2.h"
#include "Vector3.h"

namespace Math {

	struct Vector4 {
		float X, Y, Z, W;

		Vector4(float scalar)
			: X(scalar), Y(scalar), Z(scalar), W(scalar) {
		}

		Vector4(float x, float y, float z, float w)
			: X(x), Y(y), Z(z), W(w) {
		}

		Vector4(const Vector3& xyz, float w)
			: X(xyz.X), Y(xyz.Y), Z(xyz.Z), W(w) {
		}

		Vector2 GetXY() const {
			return Vector2(X, Y);
		}

		Vector3 GetXYZ() const {
			return Vector3(X, Y, Z);
		}

		void SetXYZ(const Vector3& value) {
			X = value.X;
			Y = value.Y;
			Z = value.Z;
		}

		static Vector4 Add(const Vector4& a, const Vector4& b) {
			return Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
		}

		static Vector4 Multiply(const Vector4& vector, float scalar) {
			return Vector4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
		}

		Vector4 operator+(const Vector4& b) const {
			return Add(*this, b);
		}

		Vector4 operator*(float scalar) const {
			return Multiply(*this, scalar);
		}
	};

}