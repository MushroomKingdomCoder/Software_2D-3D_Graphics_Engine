#pragma once
#include "Indexer3D.h"
#include "PixelShaders.h"
#include "Matrix.h"
#include "Vector.h"
#include "Math.h"
#include "Triangle.h"
#include <vector>

template <typename vertex>
class Object3D
{
private:
	TriangleIndexer<vertex> TriangleModel;
private:
	float rot_x = 0.0f;
	float rot_y = 0.0f;
	float rot_z = 0.0f;
	fVector3D Position = { 0,0,0 };
	fMatrix3D Rotation = fMatrix3D::Identity();
private:
	fMatrix3D RotationMatrix()
	{
		return fMatrix3D(
			fMatrix3D::RotationX(rot_x) *
			fMatrix3D::RotationY(rot_y) *
			fMatrix3D::RotationZ(rot_z) *
			fMatrix3D::Identity());
	}

public:
	Object3D(TriangleIndexer<vertex> triangle_model, fVector3D pos = { 0,0,0 })
		:
		TriangleModel(triangle_model),
		Position(pos)
	{}
	TriangleIndexer<vertex> GetTriangleModel() const
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
public:
	void RotateX(const float radians)
	{
		rot_x = angle_wrap(rot_x + radians);
		Rotation = RotationMatrix();
	}
	void RotateY(const float radians)
	{
		rot_y = angle_wrap(rot_y + radians);
		Rotation = RotationMatrix();
	}
	void RotateZ(const float radians)
	{
		rot_z = angle_wrap(rot_z + radians);
		Rotation = RotationMatrix();
	}
	const fMatrix3D& GetRotationMatrix()
	{
		return Rotation;
	}
	void Move(const fVector3D& d_vec3)
	{
		Position += d_vec3;
	}
	void SetPosition(const fVector3D& pos)
	{
		Position = pos;
	}
	const fVector3D& GetPosition() const
	{
		return Position;
	}
public:
	static Object3D MakeCube(float size, fVector3D pos = { 0,0,0 }) {
		const float s = size / 2.0f;
		return mObject3D(
			TriangleIndexer<mpsVERTEX>{
			std::vector<mpsVERTEX>{
				{-s, -s, s}, { s,-s,s }, { s,s,s }, { -s,s,s },
				{ -s,-s,-s }, { s,-s,-s }, { s,s,-s }, { -s,s,-s } },
				std::vector<Triangle<int>>{
					{1, 2, 0}, { 3,0,2 },
					{ 0,3,4 }, { 7,4,3 },
					{ 0,4,1 }, { 5,1,4 },
					{ 1,5,2 }, { 6,2,5 },
					{ 5,4,6 }, { 7,6,4 },
					{ 2,6,3 }, { 7,3,6 } }
		}, pos);
	}
	static Object3D MakeTexturedCube(float size, fVector3D pos = { 0,0,0 })
	{
		const float s = size / 2.0f;
		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			std::vector<tpsVERTEX>{
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
	static Object3D MakeSkinnedCube(float size, fVector3D pos = { 0,0,0 })
	{
		const float s = size / 2.0f;
		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			std::vector<tpsVERTEX>{
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
	static Object3D MakeWrappingCube(float size, fVector3D pos = { 0,0,0 })
	{

		const float s = size / 2.0f;
		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			std::vector<tpsVERTEX>{
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
	static Object3D MakeBlendingCube(float size, std::vector<Color> colors, fVector3D pos = { 0,0,0 })
	{
		const float s = size / 2.0f;
		return vbObject3D(
		TriangleIndexer<vbpsVERTEX>{
		std::vector<vbpsVERTEX>{
		{-s,-s,s,colors[0]},	{s,-s,s,colors[1]},		{s,s,s,colors[2]},		{-s,s,s,colors[3]},
		{-s,-s,-s,colors[4]},	{s,-s,-s,colors[5]},	{s,s,-s,colors[6]},		{-s,s,-s,colors[7]} },
		std::vector<Triangle<int>>{
		{1,2,0},	{3,0,2},
		{0,3,4},	{7,4,3},
		{0,4,1},	{5,1,4},
		{1,5,2},	{6,2,5},
		{5,4,6},	{7,6,4},
		{2,6,3},	{7,3,6} } 
		}, pos);
	}
	static Object3D MakeTeselatedPlane(const iVector2D teselations, const float width, const float depth, fVector3D pos = { 0,0,0 })
	{
		std::vector<mpsVERTEX> vtxes;
		std::vector<Triangle<int>> triangles;
		const float delta_w = width / teselations.X;
		const float delta_d = depth / teselations.Y;
		float cur_d = 0.0f;
		for (int y = 0; y < teselations.Y - 1; ++y, cur_d += delta_d) {
			float cur_w = 0.0f;
			for (int x = 0; x < teselations.X - 1; ++x, cur_w += delta_w) {
				vtxes.emplace_back(mpsVERTEX(cur_w, 0, cur_d));
				const int cur_vtx = y * teselations.X + x;
				triangles.emplace_back(cur_vtx + 1, cur_vtx + teselations.X + 1, cur_vtx);
				triangles.emplace_back(cur_vtx + teselations.X, cur_vtx, cur_vtx + teselations.X + 1);
				triangles.emplace_back(cur_vtx + 1, cur_vtx, cur_vtx + teselations.X + 1);
				triangles.emplace_back(cur_vtx + teselations.X, cur_vtx + teselations.X + 1, cur_vtx);
			}
			vtxes.emplace_back(mpsVERTEX(cur_w, 0, cur_d));
		} cur_d += delta_d;
		float cur_w = 0.0f;
		for (int x = 0; x <= teselations.X; ++x, cur_w += delta_w) {
			vtxes.emplace_back(mpsVERTEX(cur_w, 0, cur_d));
		}
		for (auto& v : vtxes) {
			v.pos -= fVector3D(width / 2, 0, depth / 2);
		}

		return mObject3D(
			TriangleIndexer<mpsVERTEX>{
			vtxes, triangles}, pos);
	}
	static Object3D MakeTeselatedSkinnedPlane(const iVector2D teselations, const float width, const float depth, fVector3D pos = { 0,0,0 })
	{
		std::vector<tpsVERTEX> vtxes;
		std::vector<Triangle<int>> triangles;
		const float delta_w = width / teselations.X;
		const float delta_d = depth / teselations.Y;
		float cur_d = 0.0f;
		for (int y = 0; y < teselations.Y - 1; ++y, cur_d += delta_d) {
			float cur_w = 0.0f;
			for (int x = 0; x < teselations.X - 1; ++x, cur_w += delta_w) {
				vtxes.emplace_back(tpsVERTEX(cur_w, 0, cur_d, cur_w / width, 1.0f - (cur_d / depth)));
				const int cur_vtx = y * teselations.X + x;
				triangles.emplace_back(cur_vtx + 1, cur_vtx + teselations.X + 1, cur_vtx);
				triangles.emplace_back(cur_vtx + teselations.X, cur_vtx, cur_vtx + teselations.X + 1);
				triangles.emplace_back(cur_vtx + 1, cur_vtx, cur_vtx + teselations.X + 1);
				triangles.emplace_back(cur_vtx + teselations.X, cur_vtx + teselations.X + 1, cur_vtx);
			}
			vtxes.emplace_back(tpsVERTEX(cur_w, 0, cur_d, cur_w / width, 1.0f - (cur_d / depth)));
		} cur_d += delta_d;
		float cur_w = 0.0f;
		for (int x = 0; x <= teselations.X; ++x, cur_w += delta_w) {
			vtxes.emplace_back(tpsVERTEX(cur_w, 0, cur_d, cur_w / width, 1.0f - (cur_d / depth)));
		}
		for (auto& v : vtxes) {
			v.pos -= fVector3D(width / 2, 0, depth / 2);
		}

		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			vtxes, triangles}, pos);
	}
};
typedef Object3D<tpsVERTEX> tObject3D;
typedef Object3D<vbpsVERTEX> vbObject3D;
typedef Object3D<mpsVERTEX> mObject3D;



