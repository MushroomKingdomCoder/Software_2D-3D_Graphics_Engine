#pragma once
#include "Graphics.h"
#include "Vector.h"

class NDCBuffer
{
private:
	int xFactor;
	int yFactor;

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
};