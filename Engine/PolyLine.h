#pragma once
#include "Rect.h"
#include "Vector.h"
#include "Matrix.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

class PolyLine
{
private:
	std::vector<fVector2D> Model;
	fVector2D Pos = { 0,0 };
	float Scale = 1.0f;
	float Rotation = 0.0f;
	fMatrix2Dplus Transformation = fMatrix2Dplus::Identity();

public:
	PolyLine() = default;
	PolyLine(std::vector<fVector2D> verticies, fVector2D pos = { 0,0 });
	void Move(fVector2D delta_pos);
	fVector2D GetPos() const;
	void SetPos(fVector2D pos);
	std::vector<fVector2D> GetRendered();
	void ScaleBy(const float scale);
	void SetScale(float scl);
	float GetScale() const;
	const std::vector<fVector2D>& GetModel() const;
	fRect GetRect() const;
	void Rotate(const float radians);
	void SetRotation(const float radians);
	float GetRotation() const;
public:
	static PolyLine MakeSquare(float size, fVector2D pos = { 0,0 });
	static PolyLine MakeCircle(float radius, fVector2D pos = { 0,0 });
	static PolyLine MakeEllipse(float radius, float x_stretch = 1.0f, float y_stretch = 1.0f, fVector2D pos = { 0,0 });
	static PolyLine MakeStar(int nFlares, float i_rad, float o_rad, fVector2D pos = { 0,0 });
	static PolyLine MakeRectangle(float width, float height, fVector2D pos = { 0,0 });
};
