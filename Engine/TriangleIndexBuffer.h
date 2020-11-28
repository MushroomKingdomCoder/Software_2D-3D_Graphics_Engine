#pragma once
#include "Vector.h"
#include <vector>

struct TriangleIndexBuffer
{
	std::vector<fVector3D> Points;
	std::vector<std::vector<int>> Triangles;
};
