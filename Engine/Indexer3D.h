#pragma once
#include "Triangle.h"
#include "Vector.h"
#include <vector>
#include <cassert>

template <typename vertex>
struct TriangleIndexer
{
	std::vector<vertex> Verticies;
	std::vector<Triangle<size_t>> Triangles;
public:
	TriangleIndexer() = default;
	template <typename mVertex>  
	TriangleIndexer(TriangleIndexer<mVertex> t_model)
		:
		Triangles(t_model.Triangles)
	{
		for (const auto& v : t_model.Verticies) {
			Verticies.emplace_back(vertex(v));
		}
	}
	TriangleIndexer(std::vector<vertex> verticies, std::vector<Triangle<size_t>> triangles)
		:
		Verticies(std::move(verticies)),
		Triangles(std::move(triangles))
	{}
};


template <typename vertex>
struct LineIndexer
{
	std::vector<vertex> Verticies;
	std::vector<std::pair<size_t, size_t>> Lines;
public:
	LineIndexer() = default;
	LineIndexer(std::vector<vertex> verticies, std::vector<std::pair<size_t, size_t>> lines)
		:
		Verticies(std::move(verticies)),
		Lines(std::move(lines))
	{}
};