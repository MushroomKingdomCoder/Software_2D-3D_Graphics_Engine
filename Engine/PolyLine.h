#pragma once
#include "Rect.h"
#include "Vector2D.h"
#include "Matrix3D.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

class PolyLine
{
private:
	std::vector<fVector2D> Model;
	fVector2D Pos = { 0.0f, 0.0f };
	float Scale = 1.0f;
	float Rotation = 0.0f;

public:
	PolyLine() = default;
	PolyLine(std::vector<fVector2D> verticies, fVector2D pos = { 0,0 });
	void Move(fVector2D delta_pos);
	fVector2D GetPos() const;
	void SetPos(fVector2D pos);
	std::vector<fVector2D> GetRendered();
	float GetScale() const;
	void ScaleBy(const float scale);
	void Transform(const float scale);
	const std::vector<fVector2D>& GetModel() const;
	fRect GetRect() const;
	void Rotate(const float radians);
	void SetRotation(const float radians);
public:
	static PolyLine MakeSquare(float size, fVector2D pos = { 0,0 });
	static PolyLine MakeCircle(float radius, fVector2D pos = { 0,0 });
	static PolyLine MakeEllipse(float radius, float x_stretch = 1.0f, float y_stretch = 1.0f, fVector2D pos = { 0,0 });
	static PolyLine MakeStar(int nFlares, float i_rad, float o_rad, fVector2D pos = { 0,0 });
	static PolyLine MakeRectangle(float width, float height, fVector2D pos = { 0,0 });
};
