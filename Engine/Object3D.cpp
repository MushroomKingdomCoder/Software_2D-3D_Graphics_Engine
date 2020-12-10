#include "Object3D.h"

Object3D::Object3D(TriangleIndexer triangle_model, fVector3D pos)
	:
	TriangleModel(triangle_model),
	Position(pos)
{}

fMatrix3D Object3D::RotationMatrix()
{
	return fMatrix3D(
		fMatrix3D::RotationX(rot_x) *
		fMatrix3D::RotationY(rot_y) *
		fMatrix3D::RotationZ(rot_z) *
		fMatrix3D::Identity());
}

TriangleIndexer Object3D::GetTriangleModel() const
{
	return TriangleModel;
}

std::vector<fVector3D> Object3D::GetVerticies() const
{
	std::vector<fVector3D> pos_verts;
	for (const auto& tv : TriangleModel.Verticies) {
		pos_verts.emplace_back(tv.pos);
	}
	return pos_verts;
}

std::vector<fTextureVector> Object3D::GetTextureVerticies() const
{
	return TriangleModel.Verticies;
}

void Object3D::RotateX(const float radians)
{
	rot_x = angle_wrap(rot_x + radians);
	Rotation = RotationMatrix();
}

void Object3D::RotateY(const float radians) 
{
	rot_y = angle_wrap(rot_y + radians);
	Rotation = RotationMatrix();
}

void Object3D::RotateZ(const float radians)
{
	rot_z = angle_wrap(rot_z + radians);
	Rotation = RotationMatrix();
}

const fMatrix3D& Object3D::GetRotationMatrix()
{
	return Rotation;
}

void Object3D::Move(const fVector3D& d_vec3)
{
	Position += d_vec3;
}
void Object3D::SetPosition(const fVector3D& pos)
{
	Position = pos;
}
const fVector3D& Object3D::GetPosition() const
{
	return Position;
}

Object3D Object3D::MakeTexturedCube(float size, fVector3D pos)
{
	const float s = size / 2.0f;
	return Object3D(
		TriangleIndexer{
		std::vector<fTextureVector>{
		{-s,s,s,0,0},	{s,s,s,1,0},	{-s,-s,s,0,1},	{s,-s,s,1,1},
		{-s,s,-s,0,0},	{-s,s,s,1,0},	{-s,-s,-s,0,1},	{-s,-s,s,1,1},
		{-s,-s,s,0,0},	{s,-s,s,1,0},	{-s,-s,-s,0,1},	{s,-s,-s,1,1},
		{s,s,s,0,0},	{s,s,-s,1,0},	{s,-s,s,0,1},	{s,-s,-s,1,1},
		{s,s,-s,0,0},	{-s,s,-s,1,0},	{s,-s,-s,0,1},	{-s,-s,-s,1,1},
		{-s,s,-s,0,0},	{s,s,-s,1,0},	{-s,s,s,0,1},	{s,s,s,1,1} },
		std::vector<Triangle<int>>{
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
	TriangleIndexer{ 
	std::vector<fTextureVector>{
						{-s,s,s,0.25f,0.0f},	{-s,s,-s,0.50f,0.0f},
	{-s,s,s,0.0f,0.33f},{-s,-s,s,0.25f,0.33f},	{-s,-s,-s,0.50f,0.33f},	{-s,s,-s,0.75f,0.33f},	{-s,s,s,1.0f,0.33f},
	{s,s,s,0.0f,0.66f},	{s,-s,s,0.25f,0.66f},	{s,-s,-s,0.50f,0.66f},	{s,s,-s,0.75f,0.66f},	{s,s,s,1.0f,0.66f},
						{s,s,s,0.25f,1.0f},		{s,s,-s,0.50f,1.0f} },
	std::vector<Triangle<int>>{
							{0,1,3},	{4,3,1},
	{2,3,7},	{8,7,3},	{3,4,8},	{9,8,4},	{4,5,9},	{10,9,5},	{5,6,10},	{10,9,5},	{5,6,10},	{11,10,6},
							{8,9,12},	{13,12,9} } }, 
	pos);
}

Object3D Object3D::MakeWrappingCube(float size, fVector3D pos)
{

	const float s = size / 2.0f;
	return Object3D(
		TriangleIndexer{
		std::vector<fTextureVector>{
						{-s,s,s,1,0},	{-s,s,-s,2,0},
		{-s,s,s,0,1},	{-s,-s,s,1,1},	{-s,-s,-s,2,1},	{-s,s,-s,3,1},	{-s,s,s,4,1},
		{s,s,s,0,2},	{s,-s,s,1,2},	{s,-s,-s,2,2},	{s,s,-s,3,2},	{s,s,s,4,2},
						{s,s,s,1,3},	{s,s,-s,2,3} },
		std::vector<Triangle<int>>{
								{0,1,3},	{4,3,1},
		{2,3,7},	{8,7,3},	{3,4,8},	{9,8,4},	{4,5,9},	{10,9,5},	{5,6,10},	{10,9,5},	{5,6,10},	{11,10,6},
								{8,9,12},	{13,12,9} } },
		pos);
}
