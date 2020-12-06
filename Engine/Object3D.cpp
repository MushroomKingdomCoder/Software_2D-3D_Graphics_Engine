#include "Object3D.h"

Object3D::Object3D(std::vector<fVector3D> verts, LineIndexBuffer lib, TriangleIndexBuffer tib, fVector3D pos)
	:
	Verticies(verts),
	Position(pos),
	LIB(lib),
	TIB(tib)
{}

Object3D::Object3D(std::vector<fTextureVector> tverts, TriangleIndexBuffer tib, fVector3D pos)
	:
	tVerticies(tverts),
	TIB(tib),
	Position(pos)
{}

LineIndexBuffer Object3D::GetLIB() const
{
	return LIB;
}

TriangleIndexBuffer Object3D::GetTIB() const
{
	return TIB;
}

std::vector<fVector3D> Object3D::GetVerticies() const
{
	return Verticies;
}

std::vector<fTextureVector> Object3D::GetTextureVerticies() const
{
	return tVerticies;
}

void Object3D::RotateX(const float radians)
{
	rot_x = angle_wrap(rot_x + radians);
}

void Object3D::RotateY(const float radians) 
{
	rot_y = angle_wrap(rot_y + radians);
}

void Object3D::RotateZ(const float radians)
{
	rot_z = angle_wrap(rot_z + radians);
}

fMatrix2D Object3D::RotationZMatrix() const
{
	return  fMatrix2D::Rotation(rot_z) * 
			fMatrix2D::Identity();
}

fMatrix3D Object3D::RotationMatrix() const
{
	return fMatrix3D(
		fMatrix3D::RotationX(rot_x) *
		fMatrix3D::RotationY(rot_y) *
		fMatrix3D::RotationZ(rot_z) *
		fMatrix3D::Identity()
	);
}
void Object3D::Move(const fVector3D& d_vec3)
{
	Position += d_vec3;
}
void Object3D::SetPosition(const fVector3D& pos)
{
	Position = pos;
}
fVector3D Object3D::GetPosition() const
{
	return Position;
}

Object3D Object3D::MakeCube(float size, fVector3D pos)
{
	auto s = size / 2.0f;
	return Object3D( 
		std::vector<fVector3D>{
		{-s,-s,s},	{s,-s,s}, 
		{s,s,s},	{-s,s,s},
		{-s,-s,-s},	{s,-s,-s},
		{s,s,-s},	{-s,s,-s} },
		LineIndexBuffer{ {
		{0,1},	{0,3},	{0,4},
		{2,1},	{2,3},	{2,6},
		{5,4},	{5,6},	{5,1},
		{7,6},	{7,4},	{7,3} } },
		TriangleIndexBuffer{ {
		{0,1,3},	{2,3,1},
		{0,3,4},	{7,4,3},
		{0,4,1},	{5,1,4},
		{1,5,2},	{6,2,5},
		{5,4,6},	{7,6,4},
		{7,3,6},	{2,6,3} } },
		pos);
}

Object3D Object3D::MakeTexturedCube(float size, fVector3D pos)
{
	const float s = size / 2.0f;
	return Object3D(
		std::vector<fTextureVector>{
		{-s,s,s,0,0},	{s,s,s,1,0},	{-s,-s,s,0,1},	{s,-s,s,1,1},
		{-s,s,-s,0,0},	{-s,s,s,1,0},	{-s,-s,-s,0,1},	{-s,-s,s,1,1},
		{-s,-s,s,0,0},	{s,-s,s,1,0},	{-s,-s,-s,0,1},	{s,-s,-s,1,1},
		{s,s,s,0,0},	{s,s,-s,1,0},	{s,-s,s,0,1},	{s,-s,-s,1,1},
		{s,s,-s,0,0},	{-s,s,-s,1,0},	{s,-s,-s,0,1},	{-s,-s,-s,1,1},
		{-s,s,-s,0,0},	{s,s,-s,1,0},	{-s,s,s,0,1},	{s,s,s,1,1} },
		TriangleIndexBuffer{ {
		{0,2,1},	{3,1,2},
		{5,4,7},	{6,7,4},
		{8,10,9},	{11,9,10},
		{14,15,12},	{13,12,15},
		{19,17,18},	{16,18,17},
		{20,22,21},	{23,21,22} } },
		pos);
}

Object3D Object3D::MakeSkinnedCube(float size, fVector3D pos)
{
	const float s = size / 2.0f;
	return Object3D(
	std::vector<fTextureVector>{
						{-s,s,s,0.25f,0.0f},	{-s,s,-s,0.50f,0.0f},
	{-s,s,s,0.0f,0.33f},{-s,-s,s,0.25f,0.33f},	{-s,-s,-s,0.50f,0.33f},	{-s,s,-s,0.75f,0.33f},	{-s,s,s,1.0f,0.33f},
	{s,s,s,0.0f,0.66f},	{s,-s,s,0.25f,0.66f},	{s,-s,-s,0.50f,0.66f},	{s,s,-s,0.75f,0.66f},	{s,s,s,1.0f,0.66f},
						{s,s,s,0.25f,1.0f},		{s,s,-s,0.50f,1.0f} },
	TriangleIndexBuffer{ {
							{0,1,3},	{4,3,1},
	{2,3,7},	{8,7,3},	{3,4,8},	{9,8,4},	{4,5,9},	{10,9,5},	{5,6,10},	{10,9,5},	{5,6,10},	{11,10,6},
							{8,9,12},	{13,12,9} } }, 
	pos);
}
