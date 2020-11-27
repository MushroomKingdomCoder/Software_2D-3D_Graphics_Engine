#pragma once
#include "Vector.h"
#include <vector>

struct VertexIndexBuffer
{
	std::vector<fVector3D> Points;
	std::vector<std::pair<int, int>> Lines;
};