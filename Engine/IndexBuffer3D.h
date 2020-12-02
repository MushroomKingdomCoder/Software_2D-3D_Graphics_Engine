#pragma once
#include "Vector.h"
#include <vector>
#include <cassert>

struct IndexBuffer3D
{
	std::vector<bool> Culling;
};

struct TriangleIndexBuffer : public IndexBuffer3D
{
	std::vector<std::vector<int>> Triangles;
public:
	TriangleIndexBuffer() = default;
	TriangleIndexBuffer(std::vector<std::vector<int>> triangles)
		:
		Triangles(std::move(triangles))
	{
		for (auto& t : Triangles) {
			t.resize(3);
			for (int i = 0; i < 3; ++i) {
				assert(t[i] < Points.size());
			}
		}
		Culling.resize(Triangles.size());
	}
};

struct LineIndexBuffer : public IndexBuffer3D
{
	std::vector<std::pair<int, int>> Lines;
public:
	LineIndexBuffer() = default;
	LineIndexBuffer(std::vector<std::pair<int, int>> lines)
		:
		Lines(std::move(lines))
	{
		Culling.resize(Lines.size());
	}
};