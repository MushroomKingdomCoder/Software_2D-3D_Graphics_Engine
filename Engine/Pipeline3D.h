#pragma once
#include "Graphics.h"
#include "Sprite.h"
#include "NDCBuffer.h"
#include "Indexer3D.h"
#include "Matrix.h"
#include "Vector.h"
#include <memory>

class Pipeline3D
{
private:
	Graphics& gfx;
	NDCBuffer& ndc;
private:
	fMatrix3D const* Rotation = nullptr;
	fVector3D const* Translation = nullptr;
	Sprite const* Texture = nullptr;
private:
	void ProcessVerticies(std::vector<fTextureVector> vtxes, std::vector<Triangle<int>> triangles)
	{
		for (auto& v : vtxes) {
			v.pos = *Rotation * v.pos;
			v.pos += *Translation;
		}
		ProcessTriangleModel(vtxes, triangles);
	}
	void ProcessTriangleModel(std::vector<fTextureVector>& vtxes, std::vector<Triangle<int>>& triangles)
	{
		for (const auto& t : triangles) {
			const auto& p0 = vtxes[t.v0];
			const auto& p1 = vtxes[t.v1];
			const auto& p2 = vtxes[t.v2];
			if ((((p1.pos - p0.pos) % (p2.pos - p0.pos)).DotProduct(p0.pos) < 0.0f)) {
				ProcessTriangle(tTriangle(p0, p1, p2));
			}
		}
	}
	void ProcessTriangle(tTriangle& triangle)
	{
		ndc.Transform(triangle.v0.pos);
		ndc.Transform(triangle.v1.pos);
		ndc.Transform(triangle.v2.pos);
		RasterizeTriangle(triangle);
	}
	void RasterizeTriangle(const tTriangle& tTriangle)
	{
		gfx.DrawTexturedTriangle(tTriangle.v0, tTriangle.v1, tTriangle.v2, *Texture);
	}

public:
	// Only ever need 1 pipeline
	Pipeline3D() = delete;
	Pipeline3D(const Pipeline3D& pipe3d) = delete;
	Pipeline3D(Pipeline3D&& pipe3d) = delete;
	Pipeline3D& operator =(const Pipeline3D& pipe3d) = delete;
	Pipeline3D& operator =(Pipeline3D&& pipe3d) = delete;
	// doesn't that just make sense ;)
	Pipeline3D(Graphics& gfx, NDCBuffer& ndc)
		:
		gfx(gfx),
		ndc(ndc)
	{}
	~Pipeline3D()
	{
		delete[] Rotation;
		delete[] Translation;
		delete[] Texture;
	}
public:
	void BindRotation(const fMatrix3D& rotation)
	{
		Rotation = &rotation;
	}
	void BindTranslation(const fVector3D& translation)
	{
		Translation = &translation;
	}
	void BindTexture(const Sprite& texture)
	{
		Texture = &texture;
	}
public:
	void ProcessObject3D(const TriangleIndexer& tmodel)
	{
		ProcessVerticies(tmodel.Verticies, tmodel.Triangles);
	}
};