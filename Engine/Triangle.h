#pragma once
#include "Vector.h"

template <typename type>
class Triangle
{
public:
	type v0;
	type v1;
	type v2;
public:
	Triangle(type v0, type v1, type v2)
		:
		v0(std::move(v0)),
		v1(std::move(v1)),
		v2(std::move(v2))
	{}
};
using tTriangle = Triangle<fTextureVector>;