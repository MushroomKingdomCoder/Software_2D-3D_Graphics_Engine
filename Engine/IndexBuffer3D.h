#pragma once
#include "Vector.h"
#include <vector>
#include <cassert>

struct IndexBuffer3D
{
	std::vector<fVector3D> Points;
	std::vector<bool> Culling;
};

struct TriangleIndexBuffer : public IndexBuffer3D
{
	std::vector<std::vector<int>> Triangles;
public:
	TriangleIndexBuffer(std::vector<fVector3D> pnts, std::vector<std::vector<int>> triangles)
		:
		Triangles(std::move(triangles))
	{
		Points = std::move(pnts);
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
	LineIndexBuffer(std::vector<fVector3D> pnts, std::vector<std::pair<int, int>> lines)
		:
		Lines(std::move(lines))
	{
		Points = std::move(pnts);
		Culling.resize(Lines.size());
	}
};