#pragma once
#include "Triangle.h"
#include "Vector.h"
#include <vector>
#include <cassert>

template <typename vertex>
struct TriangleIndexer_
{
	std::vector<vertex> Verticies;
	std::vector<Triangle<int>> Triangles;
public:
	TriangleIndexer_() = default;
	TriangleIndexer_(std::vector<vertex> verticies, std::vector<Triangle<int>> triangles)
		:
		Verticies(std::move(verticies)),
		Triangles(std::move(triangles))
	{}
};
using TriangleIndexer = TriangleIndexer_<fTextureVector>;


template <typename vertex>
struct LineIndexer_
{
	std::vector<vertex> Verticies;
	std::vector<std::pair<int, int>> Lines;
public:
	LineIndexer_() = default;
	LineIndexer_(std::vector<vertex> verticies, std::vector<std::pair<int, int>> lines)
		:
		Verticies(std::move(verticies)),
		Lines(std::move(lines))
	{}
};
using LineIndexer = LineIndexer_<fVector3D>;