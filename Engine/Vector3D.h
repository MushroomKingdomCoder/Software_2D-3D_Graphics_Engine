#pragma once
#include "Vector2D.h"

template <typename type>
class Vector3D
{
public:
	type X;
	type Y;
	type Z = type(1);

public:
	Vector3D() = default;

	Vector3D(const type x, const type y, const type z = 1)
	{
		X = x; Y = y; Z = z;
	}

	template <typename sType>
	Vector3D(Vector3D<sType> S)
		:
		X((type)S.X),
		Y((type)S.Y),
		Z((type)S.Z)
	{}

	Vector3D(Vector2D<type> vec2)
		:
		X(vec2.X),
		Y(vec2.Y),
		Z(type(1))
	{}

	Vector3D operator+(const Vector3D& addend) const
	{
		return { X + addend.X, Y + addend.Y, Z + addend.Z };
	}

	Vector3D& operator+=(const Vector3D& addend)
	{
		return *this = *this + addend;
	}

	Vector3D operator*(const type scale) const
	{
		return { X * scale, Y * scale, Z * scale };
	}

	Vector3D& operator*=(const type scale)
	{
		return *this = *this * scale;
	}

	Vector3D operator-(const Vector3D& dif) const
	{
		return { X - dif.X, Y - dif.Y, Z - dif.Z };
	}

	Vector3D& operator-=(const Vector3D& dif)
	{
		return *this = *this - dif;
	}

	Vector3D operator -() const
	{
		return Vector3D(-(this->X), -(this->Y), -(this->Z));
	}

	type operator *(const Vector3D& vec) const
	{
		return type(X * vec.X + Y * vec.Y + Z * vec.Z);
	}

	Vector3D Normalized()
	{
		const type len = Length();
		if (len != (type)0.0f) {
			return *this * ((type)1.0f / len);
		}
		return { (type)0,(type)0 };
	}

	Vector3D& Normalize()
	{
		return *this = Normalized();
	}

	bool operator ==(const Vector3D& vec3) const
	{
		return (X == vec3.X && Y == vec3.Y && Z == vec3.Z);
	}

	bool operator !=(const Vector3D& vec3) const
	{
		return !(*this == vec3);
	}

public:
	const type& operator [](int i) const
	{
		i = i % 3;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else if (i == 2) {
			return Z;
		}
		return X;
	}
	type& operator [](int i)
	{
		i = i % 3;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else if (i == 2) {
			return Z;
		}
		return X;
	}
};

using fVector3D = Vector3D<float>;
using dVector3D = Vector3D<double>;
using iVector3D = Vector3D<int>;