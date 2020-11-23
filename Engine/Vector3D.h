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

	type& operator [](const int i)
	{
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else if (i == 2) {
			return Z;
		}
		else {
			//Invalid Index!
			return (type)i;
		}
	}
};