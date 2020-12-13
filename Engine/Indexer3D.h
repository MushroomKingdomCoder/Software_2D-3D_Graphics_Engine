#pragma once
#include "Triangle.h"
#include "Vector.h"
#include <vector>
#include <cassert>

template <typename vertex>
struct TriangleIndexer
{
	std::vector<vertex> Verticies;
	std::vector<Triangle<int>> Triangles;
public:
	TriangleIndexer() = default;
	TriangleIndexer(std::vector<vertex> verticies, std::vector<Triangle<int>> triangles)
		:
		Verticies(std::move(verticies)),
		Triangles(std::move(triangles))
	{}
};


template <typename vertex>
struct LineIndexer
{
	std::vector<vertex> Verticies;
	std::vector<std::pair<int, int>> Lines;
public:
	LineIndexer() = default;
	LineIndexer(std::vector<vertex> verticies, std::vector<std::pair<int, int>> lines)
		:
		Verticies(std::move(verticies)),
		Lines(std::move(lines))
	{}
};