#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector.h"

template <typename type>
inline type sq(type x)
{
	return x * x;
}

template <typename type>
inline type angle_wrap(type radians)
{
	while (radians > M_PI) {
		radians -= 2 * M_PI;
	}
	return radians;
}

template <typename type>
inline Vector3D<type> vec3_abs(const Vector3D<type>& vec3)
{
	return Vector3D<type>(abs(vec3.X), abs(vec3.Y), abs(vec3.Z));
}