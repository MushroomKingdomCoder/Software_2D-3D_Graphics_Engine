#include "Prism.h"

Prism::Prism(std::vector<fVector3D> verts, fVector3D pos)
	:
	Verticies(verts),
	Position(pos)
{}

LineIndexBuffer Prism::GetLIB() const
{
	return LineIndexBuffer{
		Verticies, {
		{0,1},	{0,3},	{0,4},
		{2,1},	{2,3},	{2,6},
		{5,4},	{5,6},	{5,1},
		{7,6},	{7,4},	{7,3}
	} };
}

TriangleIndexBuffer Prism::GetTIB() const
{
	return TriangleIndexBuffer{
		Verticies, {
		{0,1,3},	{2,3,1},
		{0,3,4},	{7,4,3},
		{0,4,1},	{5,1,4},
		{1,5,2},	{6,2,5},
		{5,4,6},	{7,6,4},
		{7,3,6},	{2,6,3}
	} };
}

void Prism::RotateX(const float radians)
{
	rot_x = angle_wrap(rot_x + radians);
}

void Prism::RotateY(const float radians) 
{
	rot_y = angle_wrap(rot_y + radians);
}

void Prism::RotateZ(const float radians)
{
	rot_z = angle_wrap(rot_z + radians);
}

fMatrix3D Prism::RotationMatrix() const
{
	return fMatrix3D(
		fMatrix3D::RotationX(rot_x) *
		fMatrix3D::RotationY(rot_y) *
		fMatrix3D::RotationZ(rot_z) *
		fMatrix3D::Identity()
	);
}
void Prism::Move(const fVector3D& d_vec3)
{
	Position += d_vec3;
}
void Prism::SetPosition(const fVector3D& pos)
{
	Position = pos;
}
fVector3D Prism::GetPosition() const
{
	return Position;
}

Prism Prism::MakeCube(float size, fVector3D pos)
{
	auto s = size / 2.0f;
	return Prism({
		{-s,-s,s},
		{s,-s,s},
		{s,s,s},
		{-s,s,s},
		{-s,-s,-s},
		{s,-s,-s},
		{s,s,-s},
		{-s,s,-s}
		}, pos);
}