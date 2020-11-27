#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

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