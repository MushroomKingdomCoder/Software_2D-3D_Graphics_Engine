#pragma once
#include "Graphics.h"
#include "Sprite.h"
#include "NDCBuffer.h"
#include "Indexer3D.h"
#include "PixelShaders.h"
#include "ZBuffer.h"
#include "Matrix.h"
#include "Vector.h"
#include <memory>
#include <functional>

template <typename pShader, typename Vertex = pShader::Vertex>
class Pipeline3D
{
private:
	Graphics& gfx;
	NDCBuffer& ndc;
	pShader& PixelShade;
	ZBuffer& zBuffer;
private:
	fMatrix3D const* pRotation = nullptr;
	fVector3D const* pTranslation = nullptr;
private:
	void ProcessVerticies(std::vector<Vertex> vtxes, std::vector<Triangle<int>> triangles)
	{
		for (auto& v : vtxes) {
			v.pos = *pRotation * v.pos;
			v.pos += *pTranslation;
		}
		ProcessTriangleModel(vtxes, triangles);
	}
	void ProcessTriangleModel(std::vector<Vertex>& vtxes, std::vector<Triangle<int>>& triangles)
	{
		for (const auto& t : triangles) {
			const auto& p0 = vtxes[t.v0];
			const auto& p1 = vtxes[t.v1];
			const auto& p2 = vtxes[t.v2];
			if ((((p1.pos - p0.pos) % (p2.pos - p0.pos)).DotProduct(p0.pos) < 0.0f)) {
				ProcessTriangle(Triangle<Vertex>(p0, p1, p2));
			}
		}
	}
	void ProcessTriangle(Triangle<Vertex>& triangle)
	{
		ndc.Transform(triangle.v0);
		ndc.Transform(triangle.v1);
		ndc.Transform(triangle.v2);
		RasterizeTriangle(triangle);
	}
	void RasterizeTriangle(const Triangle<Vertex>& tTriangle)
	{
		DrawTriangle(tTriangle.v0, tTriangle.v1, tTriangle.v2);
	}
	void DrawTriangle(const Vertex& p0, const Vertex& p1, const Vertex& p2)
	{
		const Vertex* pV0 = &p0;
		const Vertex* pV1 = &p1;
		const Vertex* pV2 = &p2;

		// Sort by y
		if (pV1->pos.Y < pV0->pos.Y) { std::swap(pV1, pV0); }
		if (pV2->pos.Y < pV1->pos.Y) { std::swap(pV2, pV1); }
		if (pV1->pos.Y < pV0->pos.Y) { std::swap(pV1, pV0); }

		if (pV1->pos.Y == pV0->pos.Y) { // Natural Flat-Top
			if (pV1->pos.X < pV0->pos.X) { std::swap(pV1, pV0); } // Sort x
			DrawFlatTopTriangle(*pV0, *pV1, *pV2);
		}
		else if (pV2->pos.Y == pV1->pos.Y) { // Natural Flat-Bottom
			if (pV2->pos.X < pV1->pos.X) { std::swap(pV2, pV1); } // Sort x
			DrawFlatBottomTriangle(*pV0, *pV1, *pV2);
		}
		else {
			// Find Splitting Vertex
			const float alpha = (pV1->pos.Y - pV0->pos.Y) / (pV2->pos.Y - pV0->pos.Y);
			const Vertex s_vtx = Vertex(*pV0).InterpolatedTo(*pV2, alpha);

			if (s_vtx.pos.X < pV1->pos.X) { // Major Left
				DrawFlatBottomTriangle(*pV0, s_vtx, *pV1);
				DrawFlatTopTriangle(s_vtx, *pV1, *pV2);
			}
			else { // Major Right
				DrawFlatBottomTriangle(*pV0, *pV1, s_vtx);
				DrawFlatTopTriangle(*pV1, s_vtx, *pV2);
			}
		}
	}
	void DrawFlatTopTriangle(const Vertex& p0, const Vertex& p1, const Vertex& p2)
	{
		const float delta_y = p2.pos.Y - p0.pos.Y;
		const Vertex dv0 = (p2 - p0) / delta_y;
		const Vertex dv1 = (p2 - p1) / delta_y;
		Vertex edge1 = p1;
		DrawFlatTriangle(p0, p1, p2, dv0, dv1, edge1);
	}
	void DrawFlatBottomTriangle(const Vertex& p0, const Vertex& p1, const Vertex& p2)
	{
		const float delta_y = p2.pos.Y - p0.pos.Y;
		const Vertex dv0 = (p1 - p0) / delta_y;
		const Vertex dv1 = (p2 - p0) / delta_y;
		Vertex edge1 = p0;
		DrawFlatTriangle(p0, p1, p2, dv0, dv1, edge1);
	}
	void DrawFlatTriangle(const Vertex& p0, const Vertex& p1, const Vertex& p2, const Vertex& dv0, const Vertex& dv1, Vertex edge1)
	{
		Vertex edge0 = p0;
		const int yStart = (int)ceil(p0.pos.Y - 0.5f);
		const int yEnd = (int)ceil(p2.pos.Y - 0.5f);
		edge0 += dv0 * (float(yStart) + 0.5f - p0.pos.Y);
		edge1 += dv1 * (float(yStart) + 0.5f - p0.pos.Y);
		for (int y = yStart; y < yEnd; ++y, edge0 += dv0, edge1 += dv1) {
			const int xStart = (int)ceil(edge0.pos.X - 0.5f);
			const int xEnd = (int)ceil(edge1.pos.X - 0.5f);
			Vertex dvl = (edge1 - edge0) / (edge1.pos.X - edge0.pos.X);
			Vertex ipos = edge0 + dvl * (float(xStart) + 0.5f - edge0.pos.X);
			for (int x = xStart; x < xEnd; ++x, ipos += dvl) {
				if (gfx.ScreenRect().ContainsPoint(iVector2D{ x,y })) {
					const float zCorrection = 1.0f / ipos.pos.Z;
					if (zBuffer.TestAndSet(x, y, zCorrection)) {
						const Vertex pCorrectionVtx = ipos * zCorrection;
						gfx.PutPixel(x, y, PixelShade(pCorrectionVtx));
					}
				}
			}
		}
	}

public:
	Pipeline3D() = delete;
	Pipeline3D(Graphics& gfx, NDCBuffer& ndc, pShader& pShader, ZBuffer& zbuf)
		:
		gfx(gfx),
		ndc(ndc),
		PixelShade(pShader),
		zBuffer(zbuf)
	{}
public:
	void BindRotation(const fMatrix3D& rotation)
	{
		pRotation = &rotation;
	}
	void BindTranslation(const fVector3D& translation)
	{
		pTranslation = &translation;
	}
public:
	void ProcessObject3D(const TriangleIndexer<Vertex>& tmodel)
	{
		ProcessVerticies(tmodel.Verticies, tmodel.Triangles);
	}
};
typedef Pipeline3D<tPIXELSHADER> tpsPIPELINE;
typedef Pipeline3D<vbPIXELSHADER> vbpsPIPELINE;
typedef Pipeline3D<mPIXELSHADER> mpsPIPELINE;

