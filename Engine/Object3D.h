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
private:
	fMatrix3Dplus RotationMatrix() const
	{
		return fMatrix3Dplus(
			fMatrix3Dplus::RotationX(rot_x) *
			fMatrix3Dplus::RotationY(rot_y) *
			fMatrix3Dplus::RotationZ(rot_z) *
			fMatrix3Dplus::Identity());
	}

public:
	template <typename object_type>
	Object3D(Object3D<object_type> obj)
		:
		TriangleModel(TriangleIndexer<vertex>(obj.GetTriangleModel())),
		rot_x(obj.GetXRotation()),
		rot_y(obj.GetYRotation()),
		rot_z(obj.GetZRotation()),
		Position(obj.GetPosition())
	{}
	Object3D(TriangleIndexer<vertex> triangle_model, fVector3D pos = { 0,0,0 })
		:
		TriangleModel(triangle_model),
		Position(pos),
		rot_x(0.0f),
		rot_y(0.0f),
		rot_z(0.0f)
	{}
	const TriangleIndexer<vertex>& GetTriangleModel() const
	{
		return TriangleModel;
	}
	TriangleIndexer<vertex>& GetTriangleModel()
	{
		return TriangleModel;
	}
	Object3D& AddNormals()
	{
		for (vertex& v : TriangleModel.Verticies) {
			std::vector<Triangle<vertex>> vtriangles;
			for (const auto& t : TriangleModel.Triangles) {
				const vertex& v0 = TriangleModel.Verticies[t.v0];
				const vertex& v1 = TriangleModel.Verticies[t.v1];
				const vertex& v2 = TriangleModel.Verticies[t.v2];
				if (v0 == v || v1 == v || v2 == v) {
					vtriangles.emplace_back(v0, v1, v2);
				}
			}
			fVector3D total_faces_unnormal = { 0,0,0 };
			for (const Triangle<vertex>& vt : vtriangles) {
				total_faces_unnormal += fVector3D((vt.v1.pos - vt.v0.pos) % (vt.v2.pos - vt.v0.pos));
			}
			v.normal = total_faces_unnormal.Normalized();
		}
		return *this;
	}
public:
	void RotateX(const float radians)
	{
		rot_x = angle_wrap(rot_x + radians);
	}
	float GetXRotation() const
	{
		return rot_x;
	}
	void RotateY(const float radians)
	{
		rot_y = angle_wrap(rot_y + radians);
	}
	float GetYRotation() const
	{
		return rot_y;
	}
	void RotateZ(const float radians)
	{
		rot_z = angle_wrap(rot_z + radians);
	}
	float GetZRotation() const
	{
		return rot_z;
	}
	fMatrix3Dplus GetRotationMatrix()
	{
		return RotationMatrix();
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
	fMatrix3Dplus GetTransformationMatrix() const
	{
		return fMatrix3Dplus::Translation(Position) * RotationMatrix();
	}
public:
	static constexpr Object3D MakeCube(float size, fVector3D pos = { 0,0,0 }) {
		const float s = size / 2.0f;
		return mObject3D(
			TriangleIndexer<mpsVERTEX>{
			std::vector<mpsVERTEX>{
				{ {-s,s,s},{0,0,1} },	{ {s,s,s},{0,0,1} },	{ {-s,-s,s},{0,0,1} },	{ {s,-s,s},{0,0,1} },
				{ {-s,s,-s},{-1,0,0} },	{ {-s,s,s},{-1,0,0} },	{ {-s,-s,-s},{-1,0,0} },{ {-s,-s,s},{-1,0,0} },
				{ {-s,-s,s},{0,-1,0} },	{ {s,-s,s},{0,-1,0} },	{ {-s,-s,-s},{0,-1,0} },{ {s,-s,-s},{0,-1,0} },
				{ {s,s,s},{1,0,0} },	{ {s,s,-s},{1,0,0} },	{ {s,-s,s},{1,0,0} },	{ {s,-s,-s},{1,0,0} },
				{ {s,s,-s},{0,0,-1} },	{ {-s,s,-s},{0,0,-1} },	{ {s,-s,-s},{0,0,-1} },	{ {-s,-s,-s},{0,0,-1} },
				{ {-s,s,-s},{0,1,0} },	{ {s,s,-s},{0,1,0} },	{ {-s,s,s},{0,1,0} },	{ {s,s,s},{0,1,0} } },
			std::vector<Triangle<size_t>>{
				{ 0,2,1 }, { 3,1,2 },
				{ 5,4,7 }, { 6,7,4 },
				{ 8,10,9 }, { 11,9,10 },
				{ 14,15,12 }, { 13,12,15 },
				{ 19,17,18 }, { 16,18,17 },
				{ 20,22,21 }, { 23,21,22 }}
		}, pos);
	}
	static constexpr Object3D MakeTexturedCube(float size, fVector3D pos = { 0,0,0 })
	{
		const float s = size / 2.0f;
		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			std::vector<tpsVERTEX>{
				{ {-s,s,s},{0,0},{0,0,1} },		{ {s,s,s},{1,0},{0,0,1} },		{ {-s,-s,s},{0,1},{0,0,1} },	{ {s,-s,s},{1,1},{0,0,1} },
				{ {-s,s,-s},{0,0},{-1,0,0} },	{ {-s,s,s},{1,0},{-1,0,0} },	{ {-s,-s,-s},{0,1},{-1,0,0} },	{ {-s,-s,s},{1,1},{-1,0,0} },
				{ {-s,-s,s},{0,0},{0,-1,0} },	{ {s,-s,s},{1,0},{0,-1,0} },	{ {-s,-s,-s},{0,1},{0,-1,0} },	{ {s,-s,-s},{1,1},{0,-1,0} },
				{ {s,s,s},{0,0},{1,0,0} },		{ {s,s,-s},{1,0},{1,0,0} },		{ {s,-s,s},{0,1},{1,0,0} },		{ {s,-s,-s},{1,1},{1,0,0} },
				{ {s,s,-s},{0,0},{0,0,-1} },	{ {-s,s,-s},{1,0},{0,0,-1} },	{ {s,-s,-s},{0,1},{0,0,-1} },	{ {-s,-s,-s},{1,1},{0,0,-1} },
				{ {-s,s,-s},{0,0},{0,1,0} },	{ {s,s,-s},{1,0},{0,1,0} },		{ {-s,s,s},{0,1},{0,1,0} },		{ {s,s,s},{1,1},{0,1,0} }},
			std::vector<Triangle<size_t>>{
				{0,2,1},	{3,1,2},
				{5,4,7},	{6,7,4},
				{8,10,9},	{11,9,10},
				{14,15,12},	{13,12,15},
				{19,17,18},	{16,18,17},
				{20,22,21},	{23,21,22} 
			} }, pos);
	}
	static constexpr Object3D MakeSkinnedCube(float size, fVector3D pos = { 0,0,0 })
	{
		const float s = size / 2.0f;
		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			std::vector<tpsVERTEX>{
				{ {-s,s,s},{0,0.33f},{0,0,1} },			{ {s,s,s},{0,0.67f},{0,0,1} },			{ {-s,-s,s},{0.25f,0.33f},{0,0,1} },	{ {s,-s,s},{0.25f,0.67f},{0,0,1} },
				{ {-s,s,-s},{0.5f,0},{-1,0,0} },		{ {-s,s,s},{0.25f,0},{-1,0,0} },		{ {-s,-s,-s},{0.5f,0.33f},{-1,0,0} },	{ {-s,-s,s},{0.25f,0.33f},{-1,0,0} },
				{ {-s,-s,s},{0.25f,0.33f},{0,-1,0} },	{ {s,-s,s},{0.25f,0.67f},{0,-1,0} },	{ {-s,-s,-s},{0.5f,0.33f},{0,-1,0} },	{ {s,-s,-s},{0.5f,0.67f},{0,-1,0} },
				{ {s,s,s},{0.25f,1},{1,0,0} },			{ {s,s,-s},{0.5f,1},{1,0,0} },			{ {s,-s,s},{0.25f,0.67f},{1,0,0} },		{ {s,-s,-s},{0.5f,0.67f},{1,0,0} },
				{ {s,s,-s},{0.75f,0.67f},{0,0,-1} },	{ {-s,s,-s},{0.75f,0.33f},{0,0,-1} },	{ {s,-s,-s},{0.5f,0.67f},{0,0,-1} },	{ {-s,-s,-s},{0.5f,0.33f},{0,0,-1} },
				{ {-s,s,-s},{0.75,0.33f},{0,1,0} },		{ {s,s,-s},{0.75f,0.67f},{0,1,0} },		{ {-s,s,s},{1,0.33f},{0,1,0} },			{ {s,s,s},{1,0.67f},{0,1,0} },},
			std::vector<Triangle<size_t>>{
				{ 0,2,1 },		{ 3,1,2 },
				{ 5,4,7 },		{ 6,7,4 },
				{ 8,10,9 },		{ 11,9,10 },
				{ 14,15,12 },	{ 13,12,15 },
				{ 19,17,18 },	{ 16,18,17 },
				{ 20,22,21 },	{ 23,21,22 }
			} }, pos);
	}
	static constexpr Object3D MakeHollowCube(float size, fVector3D pos = { 0,0,0 })
	{
		const float s = size / 2.0f;
		return mObject3D(
			TriangleIndexer<mpsVERTEX>{
			std::vector<mpsVERTEX>{
				{ {-s,s,s},{0,0,1} },	{ {s,s,s},{0,0,1} },	{ {-s,-s,s},{0,0,1} },	{ {s,-s,s},{0,0,1} },
				{ {-s,s,-s},{-1,0,0} },	{ {-s,s,s},{-1,0,0} },	{ {-s,-s,-s},{-1,0,0} },{ {-s,-s,s},{-1,0,0} },
				{ {-s,-s,s},{0,-1,0} },	{ {s,-s,s},{0,-1,0} },	{ {-s,-s,-s},{0,-1,0} },{ {s,-s,-s},{0,-1,0} },
				{ {s,s,s},{1,0,0} },	{ {s,s,-s},{1,0,0} },	{ {s,-s,s},{1,0,0} },	{ {s,-s,-s},{1,0,0} },
				{ {s,s,-s},{0,0,-1} },	{ {-s,s,-s},{0,0,-1} },	{ {s,-s,-s},{0,0,-1} },	{ {-s,-s,-s},{0,0,-1} },
				{ {-s,s,-s},{0,1,0} },	{ {s,s,-s},{0,1,0} },	{ {-s,s,s},{0,1,0} },	{ {s,s,s},{0,1,0} },
					// duplicates
				{ {-s,s,s},{0,0,-1} },	{ {s,s,s},{0,0,-1} },	{ {-s,-s,s},{0,0,-1} },	{ {s,-s,s},{0,0,-1} },
				{ {-s,s,-s},{1,0,0} },	{ {-s,s,s},{1,0,0} },	{ {-s,-s,-s},{1,0,0} },	{ {-s,-s,s},{1,0,0} },
				{ {-s,-s,s},{0,1,0} },	{ {s,-s,s},{0,1,0} },	{ {-s,-s,-s},{0,1,0} },	{ {s,-s,-s},{0,1,0} },
				{ {s,s,s},{-1,0,0} },	{ {s,s,-s},{-1,0,0} },	{ {s,-s,s},{-1,0,0} },	{ {s,-s,-s},{-1,0,0} },
				{ {s,s,-s},{0,0,1} },	{ {-s,s,-s},{0,0,1} },	{ {s,-s,-s},{0,0,1} },	{ {-s,-s,-s},{0,0,1} },
				{ {-s,s,-s},{0,-1,0} },	{ {s,s,-s},{0,-1,0} },	{ {-s,s,s},{0,-1,0} },	{ {s,s,s},{0,-1,0} } },
			std::vector<Triangle<size_t>>{
				{ 0,2,1 }, { 3,1,2 },
				{ 5,4,7 }, { 6,7,4 },
				{ 8,10,9 }, { 11,9,10 },
				{ 14,15,12 }, { 13,12,15 },
				{ 19,17,18 }, { 16,18,17 },
				{ 20,22,21 }, { 23,21,22 },
					// duplicates
				{ 24,25,26 }, { 27,26,25 },
				{ 29,31,28 }, { 30,28,31 },
				{ 32,33,34 }, { 35,34,33 },
				{ 38,36,39 }, { 37,39,36 },
				{ 43,42,41 }, { 40,41,42 },
				{ 44,45,46 }, { 47,46,45 } }
		}, pos);
	}
	static constexpr Object3D MakeHollowTexturedCube(float size, fVector3D pos = { 0,0,0 })
	{
		const float s = size / 2.0f;
		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			std::vector<tpsVERTEX>{
				{ {-s,s,s},{0,0},{0,0,1} },		{ {s,s,s},{1,0},{0,0,1} },		{ {-s,-s,s},{0,1},{0,0,1} },	{ {s,-s,s},{1,1},{0,0,1} },
				{ {-s,s,-s},{0,0},{-1,0,0} },	{ {-s,s,s},{1,0},{-1,0,0} },	{ {-s,-s,-s},{0,1},{-1,0,0} },	{ {-s,-s,s},{1,1},{-1,0,0} },
				{ {-s,-s,s},{0,0},{0,-1,0} },	{ {s,-s,s},{1,0},{0,-1,0} },	{ {-s,-s,-s},{0,1},{0,-1,0} },	{ {s,-s,-s},{1,1},{0,-1,0} },
				{ {s,s,s},{0,0},{1,0,0} },		{ {s,s,-s},{1,0},{1,0,0} },		{ {s,-s,s},{0,1},{1,0,0} },		{ {s,-s,-s},{1,1},{1,0,0} },
				{ {s,s,-s},{0,0},{0,0,-1} },	{ {-s,s,-s},{1,0},{0,0,-1} },	{ {s,-s,-s},{0,1},{0,0,-1} },	{ {-s,-s,-s},{1,1},{0,0,-1} },
				{ {-s,s,-s},{0,0},{0,1,0} },	{ {s,s,-s},{1,0},{0,1,0} },		{ {-s,s,s},{0,1},{0,1,0} },		{ {s,s,s},{1,1},{0,1,0} },
					// duplicates
				{ {-s,s,s},{0,0},{0,0,-1} },	{ {s,s,s},{1,0},{0,0,-1} },		{ {-s,-s,s},{0,1},{0,0,-1} },	{ {s,-s,s},{1,1},{0,0,-1} },
				{ {-s,s,-s},{0,0},{1,0,0} },	{ {-s,s,s},{1,0},{1,0,0} },		{ {-s,-s,-s},{0,1},{1,0,0} },	{ {-s,-s,s},{1,1},{1,0,0} },
				{ {-s,-s,s},{0,0},{0,1,0} },	{ {s,-s,s},{1,0},{0,1,0} },		{ {-s,-s,-s},{0,1},{0,1,0} },	{ {s,-s,-s},{1,1},{0,1,0} },
				{ {s,s,s},{0,0},{-1,0,0} },		{ {s,s,-s},{1,0},{-1,0,0} },	{ {s,-s,s},{0,1},{-1,0,0} },	{ {s,-s,-s},{1,1},{-1,0,0} },
				{ {s,s,-s},{0,0},{0,0,1} },		{ {-s,s,-s},{1,0},{0,0,1} },	{ {s,-s,-s},{0,1},{0,0,1} },	{ {-s,-s,-s},{1,1},{0,0,1} },
				{ {-s,s,-s},{0,0},{0,-1,0} },	{ {s,s,-s},{1,0},{0,-1,0} },	{ {-s,s,s},{0,1},{0,-1,0} },	{ {s,s,s},{1,1},{0,-1,0} } },
			std::vector<Triangle<size_t>>{
				{ 0,2,1 }, { 3,1,2 },
				{ 5,4,7 }, { 6,7,4 },
				{ 8,10,9 }, { 11,9,10 },
				{ 14,15,12 }, { 13,12,15 },
				{ 19,17,18 }, { 16,18,17 },
				{ 20,22,21 }, { 23,21,22 },
					// duplicates
				{ 24,25,26 }, { 27,26,25 },
				{ 29,31,28 }, { 30,28,31 },
				{ 32,33,34 }, { 35,34,33 },
				{ 38,36,39 }, { 37,39,36 },
				{ 43,42,41 }, { 40,41,42 },
				{ 44,45,46 }, { 47,46,45 }
			} }, pos);
	}
	static constexpr Object3D MakeHollowSkinnedCube(float size, fVector3D pos = { 0,0,0 })
	{
		const float s = size / 2.0f;
		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			std::vector<tpsVERTEX>{
				{ {-s,s,s},{0,0.33f},{0,0,1} },			{ {s,s,s},{0,0.67f},{0,0,1} },			{ {-s,-s,s},{0.25f,0.33f},{0,0,1} },	{ {s,-s,s},{0.25f,0.67f},{0,0,1} },
				{ {-s,s,-s},{0.5f,0},{-1,0,0} },		{ {-s,s,s},{0.25f,0},{-1,0,0} },		{ {-s,-s,-s},{0.5f,0.33f},{-1,0,0} },	{ {-s,-s,s},{0.25f,0.33f},{-1,0,0} },
				{ {-s,-s,s},{0.25f,0.33f},{0,-1,0} },	{ {s,-s,s},{0.25f,0.67f},{0,-1,0} },	{ {-s,-s,-s},{0.5f,0.33f},{0,-1,0} },	{ {s,-s,-s},{0.5f,0.67f},{0,-1,0} },
				{ {s,s,s},{0.25f,1},{1,0,0} },			{ {s,s,-s},{0.5f,1},{1,0,0} },			{ {s,-s,s},{0.25f,0.67f},{1,0,0} },		{ {s,-s,-s},{0.5f,0.67f},{1,0,0} },
				{ {s,s,-s},{0.75f,0.67f},{0,0,-1} },	{ {-s,s,-s},{0.75f,0.33f},{0,0,-1} },	{ {s,-s,-s},{0.5f,0.67f},{0,0,-1} },	{ {-s,-s,-s},{0.5f,0.33f},{0,0,-1} },
				{ {-s,s,-s},{0.75,0.33f},{0,1,0} },		{ {s,s,-s},{0.75f,0.67f},{0,1,0} },		{ {-s,s,s},{1,0.33f},{0,1,0} },			{ {s,s,s},{1,0.67f},{0,1,0} },
					// duplicates
				{ {-s,s,s},{0,0.33f},{0,0,-1} },		{ {s,s,s},{0,0.67f},{0,0,-1} },			{ {-s,-s,s},{0.25f,0.33f},{0,0,-1} },	{ {s,-s,s},{0.25f,0.67f},{0,0,-1} },
				{ {-s,s,-s},{0.5f,0},{1,0,0} },			{ {-s,s,s},{0.25f,0},{1,0,0} },			{ {-s,-s,-s},{0.5f,0.33f},{1,0,0} },	{ {-s,-s,s},{0.25f,0.33f},{1,0,0} },
				{ {-s,-s,s},{0.25f,0.33f},{0,1,0} },	{ {s,-s,s},{0.25f,0.67f},{0,1,0} },		{ {-s,-s,-s},{0.5f,0.33f},{0,1,0} },	{ {s,-s,-s},{0.5f,0.67f},{0,1,0} },
				{ {s,s,s},{0.25f,1},{-1,0,0} },			{ {s,s,-s},{0.5f,1},{-1,0,0} },			{ {s,-s,s},{0.25f,0.67f},{-1,0,0} },	{ {s,-s,-s},{0.5f,0.67f},{-1,0,0} },
				{ {s,s,-s},{0.75f,0.67f},{0,0,1} },		{ {-s,s,-s},{0.75f,0.33f},{0,0,1} },	{ {s,-s,-s},{0.5f,0.67f},{0,0,1} },		{ {-s,-s,-s},{0.5f,0.33f},{0,0,1} },
				{ {-s,s,-s},{0.75,0.33f},{0,-1,0} },	{ {s,s,-s},{0.75f,0.67f},{0,-1,0} },	{ {-s,s,s},{1,0.33f},{0,-1,0} },		{ {s,s,s},{1,0.67f},{0,-1,0} } },
			std::vector<Triangle<size_t>>{
				{ 0,2,1 }, { 3,1,2 },
				{ 5,4,7 }, { 6,7,4 },
				{ 8,10,9 }, { 11,9,10 },
				{ 14,15,12 }, { 13,12,15 },
				{ 19,17,18 }, { 16,18,17 },
				{ 20,22,21 }, { 23,21,22 },
					// duplicates
				{ 24,25,26 }, { 27,26,25 },
				{ 29,31,28 }, { 30,28,31 },
				{ 32,33,34 }, { 35,34,33 },
				{ 38,36,39 }, { 37,39,36 },
				{ 43,42,41 }, { 40,41,42 },
				{ 44,45,46 }, { 47,46,45 }
			} }, pos);
	}
	static constexpr Object3D MakeTeselatedPlane(const iVector2D teselations, const float width, const float depth, fVector3D pos = { 0,0,0 })
	{
		std::vector<mpsVERTEX> vtxes;
		std::vector<Triangle<size_t>> triangles;
		const float delta_w = width / teselations.X;
		const float delta_d = depth / teselations.Y;
		// topside
		float cur_d = 0.0f;
		for (int y = 0; y < teselations.Y - 1; ++y, cur_d += delta_d) {
			float cur_w = 0.0f;
			for (int x = 0; x < teselations.X - 1; ++x, cur_w += delta_w) {
				vtxes.emplace_back(mpsVERTEX(cur_w, 0, cur_d));
				const int cur_vtx = y * teselations.X + x;
				triangles.emplace_back(cur_vtx + 1, cur_vtx + teselations.X + 1, cur_vtx);
				triangles.emplace_back(cur_vtx + teselations.X, cur_vtx, cur_vtx + teselations.X + 1);
			} cur_w += delta_w;
			vtxes.emplace_back(mpsVERTEX(cur_w, 0, cur_d));
		} cur_d += delta_d;
		float cur_w = 0.0f;
		for (int x = 0; x < teselations.X - 1; ++x, cur_w += delta_w) {
			vtxes.emplace_back(mpsVERTEX(cur_w, 0, cur_d));
		} cur_w += delta_w;
		vtxes.emplace_back(mpsVERTEX(cur_w, 0, cur_d));

		// underside
		const unsigned int vStart = teselations.X * teselations.Y;
		for (size_t i = 0u; i < vStart; ++i) {
			vtxes.emplace_back();
			vtxes.back() = vtxes.at(i);
		}
		for (int y = 0; y < teselations.Y - 1; ++y, cur_d += delta_d) {
			for (int x = 0; x < teselations.X - 1; ++x, cur_w += delta_w) {
				const int cur_vtx = vStart + (y * teselations.X + x);
				triangles.emplace_back(cur_vtx + 1, cur_vtx, cur_vtx + teselations.X + 1);
				triangles.emplace_back(cur_vtx + teselations.X, cur_vtx + teselations.X + 1, cur_vtx);
			}
		}

		for (auto& v : vtxes) {
			v.pos -= fVector3D(width / 2, 0, depth / 2);
		}

		return mObject3D(
			TriangleIndexer<mpsVERTEX>{
			vtxes, triangles}, pos);
	}
	static constexpr Object3D MakeTeselatedSkinnedPlane(const iVector2D teselations, const float width, const float depth, fVector3D pos = { 0,0,0 })
	{
		std::vector<tpsVERTEX> vtxes;
		std::vector<Triangle<size_t>> triangles;
		const float delta_w = width / teselations.X;
		const float delta_d = depth / teselations.Y;
		// topside
		float cur_d = 0.0f;
		for (int y = 0; y < teselations.Y - 1; ++y, cur_d += delta_d) {
			float cur_w = 0.0f;
			for (int x = 0; x < teselations.X - 1; ++x, cur_w += delta_w) {
				vtxes.emplace_back(tpsVERTEX({ cur_w, 0, cur_d }, { cur_w / width, 1.0f - (cur_d / depth) }, { 0,1,0 }));
				const int cur_vtx = y * teselations.X + x;
				triangles.emplace_back(cur_vtx + 1, cur_vtx + teselations.X + 1, cur_vtx);
				triangles.emplace_back(cur_vtx + teselations.X, cur_vtx, cur_vtx + teselations.X + 1);
			} cur_w += delta_w;
			vtxes.emplace_back(tpsVERTEX({ cur_w, 0, cur_d }, { cur_w / width, 1.0f - (cur_d / depth) }, { 0,1,0 }));
		} cur_d += delta_d;
		float cur_w = 0.0f;
		for (int x = 0; x < teselations.X - 1; ++x, cur_w += delta_w) {
			vtxes.emplace_back(tpsVERTEX({ cur_w, 0, cur_d }, { cur_w / width, 1.0f - (cur_d / depth) }, { 0,1,0 }));
		} cur_w += delta_w;
		vtxes.emplace_back(tpsVERTEX({ cur_w, 0, cur_d }, { cur_w / width, 1.0f - (cur_d / depth) }, { 0,1,0 }));

		// underside
		const unsigned int vStart = teselations.X * teselations.Y;
		for (size_t i = 0; i < vStart; ++i) {
			vtxes.emplace_back();
			const tpsVERTEX v = vtxes.at(i);
			vtxes.back() = tpsVERTEX(v.pos, v.tpos, -v.normal);
		}
		for (int y = 0; y < teselations.Y - 1; ++y, cur_d += delta_d) {
			for (int x = 0; x < teselations.X - 1; ++x, cur_w += delta_w) {
				const int cur_vtx = vStart + (y * teselations.X + x);
				triangles.emplace_back(cur_vtx + 1, cur_vtx, cur_vtx + teselations.X + 1);
				triangles.emplace_back(cur_vtx + teselations.X, cur_vtx + teselations.X + 1, cur_vtx);
			}
		}

		for (auto& v : vtxes) {
			v.pos -= fVector3D(width / 2, 0, depth / 2);
		}

		return tObject3D(
			TriangleIndexer<tpsVERTEX>{
			vtxes, triangles}, pos);
	}
	static constexpr Object3D MakePlane(const float width, const float depth, fVector3D pos = { 0,0,0 })
	{
		return MakeTeselatedPlane({ 2,2 }, width, depth, pos);
	}
	static constexpr Object3D MakeSkinnedPlane(const float width, const float depth, fVector3D pos = { 0,0,0 })
	{
		return MakeTeselatedSkinnedPlane({ 2,2 }, width, depth, pos);
	}
	static constexpr Object3D MakeSphere(float radius, unsigned int depth, fVector3D pos = { 0,0,0 })
	{
		std::vector<dVertex> vertexes;
		std::vector<Triangle<size_t>> triangles;
		initialize_sphere(vertexes, triangles, depth);
		for (auto& v : vertexes) {
			v.pos *= radius;
			v.pos.W = 1.0f;
		}
		return mObject3D(TriangleIndexer<mpsVERTEX>(vertexes, triangles), pos);
	}
	static constexpr Object3D MakeSkinnedSphere(float radius, unsigned int latDiv, unsigned int longDiv, fVector3D pos = { 0,0,0 })
	{
		const fVector3D base = { 0.0f,0.0f,radius };
		const float lattitudeAngle = (float)M_PI / latDiv;
		const float longitudeAngle = 2.0f * (float)M_PI / longDiv;
		std::vector<tpsVERTEX> vertices;
		for (unsigned int iLat = 0; iLat <= latDiv; iLat++) {
			const auto latBase = fMatrix3D::RotationX(lattitudeAngle * iLat) * base;
			for (unsigned int iLong = 0; iLong <= longDiv; iLong++) {
				vertices.emplace_back(tpsVERTEX(fMatrix3D::RotationZ(longitudeAngle * iLong) * latBase, (float)iLong / float(longDiv), (float)iLat / float(latDiv)));
			}
		}
		const auto calcIdx = [latDiv, longDiv](int iLat, int iLong) { return iLat * (longDiv + 1) + iLong; };
		std::vector<Triangle<size_t>> triangles;
		for (unsigned int iLat = 0; iLat < latDiv; iLat++) {
			for (unsigned int iLong = 0; iLong < longDiv; iLong++) {
				triangles.emplace_back(Triangle<size_t>(calcIdx(iLat, iLong), calcIdx(iLat + 1, iLong), calcIdx(iLat, iLong + 1)));
				triangles.emplace_back(Triangle<size_t>(calcIdx(iLat, iLong + 1), calcIdx(iLat + 1, iLong), calcIdx(iLat + 1, iLong + 1)));
			}
		}
		return tObject3D(TriangleIndexer<tpsVERTEX>(vertices, triangles), pos);
	}
	static constexpr Object3D MakeNonIcosphere(float radius, unsigned int latDiv, unsigned int longDiv, fVector3D pos = { 0,0,0 })
	{
		const fVector3D base = { 0.0f,0.0f,radius };
		const float lattitudeAngle = (float)M_PI / latDiv;
		const float longitudeAngle = 2.0f * (float)M_PI / longDiv;
		std::vector<mpsVERTEX> vertices;
		for (unsigned int iLat = 1; iLat < latDiv; iLat++) {
			const auto latBase = fMatrix3D::RotationX(lattitudeAngle * iLat) * base;
			for (unsigned int iLong = 0; iLong < longDiv; iLong++) {
				vertices.emplace_back(mpsVERTEX(fMatrix3D::RotationZ(longitudeAngle * iLong) * latBase));
			}
		}
		const int iNorthPole = (int)vertices.size();
		vertices.emplace_back(mpsVERTEX(base));
		const int iSouthPole = (int)vertices.size();
		vertices.emplace_back(mpsVERTEX(-base));
		const auto calcIdx = [latDiv, longDiv](int iLat, int iLong){ return iLat * longDiv + iLong; };
		std::vector<Triangle<size_t>> triangles;
		for (unsigned int iLat = 0; iLat < latDiv - 2; iLat++) {
			for (unsigned int iLong = 0; iLong < longDiv - 1; iLong++) {
				triangles.emplace_back(Triangle<size_t>(calcIdx(iLat, iLong), calcIdx(iLat + 1, iLong), calcIdx(iLat, iLong + 1)));
				triangles.emplace_back(Triangle<size_t>(calcIdx(iLat, iLong + 1), calcIdx(iLat + 1, iLong), calcIdx(iLat + 1, iLong + 1)));
			}
			triangles.emplace_back(Triangle<size_t>(calcIdx(iLat, longDiv - 1), calcIdx(iLat + 1, longDiv - 1), calcIdx(iLat, 0)));
			triangles.emplace_back(Triangle<size_t>(calcIdx(iLat, 0), calcIdx(iLat + 1, longDiv - 1), calcIdx(iLat + 1, 0)));
		}
		for (unsigned int iLong = 0; iLong < longDiv - 1; iLong++) {
			triangles.emplace_back(Triangle<size_t>(iNorthPole, calcIdx(0, iLong), calcIdx(0, iLong + 1)));
			triangles.emplace_back(Triangle<size_t>(calcIdx(latDiv - 2, iLong + 1), calcIdx(latDiv - 2, iLong), iSouthPole));
		}
		triangles.emplace_back(Triangle<size_t>(iNorthPole, calcIdx(0, longDiv - 1), calcIdx(0, 0)));
		triangles.emplace_back(Triangle<size_t>(calcIdx(latDiv - 2, 0), calcIdx(latDiv - 2, longDiv - 1), iSouthPole));
		return mObject3D(TriangleIndexer<mpsVERTEX>(vertices, triangles), pos);
	}
};
typedef Object3D<tpsVERTEX> tObject3D;
typedef Object3D<vbpsVERTEX> vbObject3D;
typedef Object3D<mpsVERTEX> mObject3D;





