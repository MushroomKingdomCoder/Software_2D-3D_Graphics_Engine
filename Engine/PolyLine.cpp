#include "PolyLine.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

PolyLine::PolyLine(std::vector<fVector2D> verticies, fVector2D pos)
	:
	Model(verticies),
	Pos(pos)
{}

void PolyLine::Move(fVector2D delta_pos)
{
	Pos += delta_pos;
}

fVector2D PolyLine::GetPos() const
{
	return Pos;
}

void PolyLine::SetPos(fVector2D pos)
{
	Pos = pos;
}

std::vector<fVector2D> PolyLine::GetRendered()
{
	auto poly = Model;
	Transformation =
		fMatrix2Dplus::Translation(Pos) *
		fMatrix2Dplus::Scale(Scale) * 
		fMatrix2Dplus::Rotation(Rotation) * 
		fMatrix2Dplus::Identity();
	for (auto& v : poly) {
		v = Transformation * v;
	}
	return poly;
}
void PolyLine::ScaleBy(const float scale)
{
	Scale *= scale;
}

void PolyLine::SetScale(float scl)
{
	Scale = scl;
}

float PolyLine::GetScale() const
{
	return Scale;
}

const std::vector<fVector2D>& PolyLine::GetModel() const
{
	return Model;
}

fRect PolyLine::GetRect() const
{
	return fRect(Model);
}

void PolyLine::Rotate(const float radians)
{
	Rotation += radians;
}

void PolyLine::SetRotation(const float radians)
{
	Rotation = radians;
}

float PolyLine::GetRotation() const
{
	return Rotation;
}

PolyLine PolyLine::MakeSquare(float size, fVector2D pos)
{
	const float sz = size / 2;
	return PolyLine(std::vector<fVector2D>{ {-sz, sz}, { sz,sz }, { sz,-sz }, { -sz,-sz } }, pos);
}

PolyLine PolyLine::MakeCircle(float radius, fVector2D pos)
{
	return MakeEllipse(radius, 1, 1, pos);
}

PolyLine PolyLine::MakeEllipse(float radius, float x_stretch, float y_stretch, fVector2D pos)
{
	std::vector<fVector2D> Model;
	if (y_stretch > x_stretch) {
		for (int x = int(-radius * x_stretch); x < int(radius * x_stretch); ++x) {
			const float y = float(sqrt(pow(radius * x_stretch, 2) - pow(x, 2)) * y_stretch);
			Model.emplace_back(fVector2D((float)x, y));
		}
		for (int x = int(radius * x_stretch); x > int(-radius * x_stretch); --x) {
			const float y = float(-sqrt(pow(radius * x_stretch, 2) - pow(x, 2)) * y_stretch);
			Model.emplace_back(fVector2D((float)x, y));
		}
	}
	else {
		for (int y = int(-radius * y_stretch); y < int(radius * y_stretch); ++y) {
			const float x = float(sqrt(pow(radius * y_stretch, 2) - pow(y, 2)) * x_stretch);
			Model.emplace_back(fVector2D(x, (float)y));
		}
		for (int y = int(radius * y_stretch); y > int(-radius * y_stretch); --y) {
			const float x = float(-sqrt(pow(radius * y_stretch, 2) - pow(y, 2)) * x_stretch);
			Model.emplace_back(fVector2D(x, (float)y));
		}
	}
	return PolyLine(Model, pos);
}

PolyLine PolyLine::MakeStar(int nFlares, float i_rad, float o_rad, fVector2D pos)
{
	std::vector<fVector2D> Model;
	const int nVerts = nFlares * 2;
	Model.reserve(nVerts);
	const float radians = float(2.0f * M_PI / nVerts);
	for (int i = 0; i < nVerts; ++i) {
		const float radius = (i % 2 == 0) ? o_rad : i_rad;
		Model.emplace_back(fVector2D(radius * cos(i * radians), radius * sin(i * radians)));
	}

	return PolyLine(Model, pos);
}

PolyLine PolyLine::MakeRectangle(float width, float height, fVector2D pos)
{
	const float w = width / 2;
	const float h = height / 2;
	return PolyLine({ { -w,h }, { w,h }, { w,-h }, { -w,-h } }, pos);
}
