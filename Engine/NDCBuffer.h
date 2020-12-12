#pragma once
#include "Graphics.h"
#include "Vector.h"

class NDCBuffer
{
private:
	int xFactor = Graphics::ScreenWidth / 2;
	int yFactor = Graphics::ScreenHeight / 2;

public:
	NDCBuffer()
		:
		xFactor(Graphics::ScreenWidth / 2),
		yFactor(Graphics::ScreenHeight / 2)
	{}
	void Transform(fVector3D& point) const
	{
		point = GetTransformed(point);
	}
	fVector3D GetTransformed(const fVector3D& point) const
	{
		auto Point = point;
		const float zInv = 1.0f / point.Z;
		Point.X = (Point.X * zInv + 1.0f) * xFactor;
		Point.Y = (-Point.Y * zInv + 1.0f) * yFactor;
		return Point;
	}
	template <typename Vertex>
	Vertex GetTransformed(const Vertex& vtx) const
	{
		auto Vtx = vtx;
		const float zInv = 1.0f / Vtx.pos.Z;
		Vtx *= zInv;
		Vtx.pos.X = (Vtx.pos.X + 1.0f) * xFactor;
		Vtx.pos.Y = (-Vtx.pos.Y + 1.0f) * yFactor;
		Vtx.pos.Z = zInv;
		return Vtx;
	}
	template <typename Vertex>
	Vertex& Transform(Vertex& vtx)
	{
		return vtx = GetTransformed(vtx);
	}
};