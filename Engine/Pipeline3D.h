#pragma once
#include "Graphics.h"
#include "Sprite.h"
#include "NDCBuffer.h"
#include "Indexer3D.h"
#include "PixelShaders.h"
#include "VertexShaders.h"
#include "ZBuffer.h"
#include "Matrix.h"
#include "Vector.h"
#include <memory>
#include <functional>

template <class Effect3D>
class Pipeline3D
{
	typedef typename Effect3D::PIXELSHADER pShader;
	typedef typename Effect3D::VERTEXSHADER vShader;
	typedef typename vShader::VertexIn VSIn;
	typedef typename vShader::VertexOut VSOut;
private:
	Graphics& gfx;
	NDCBuffer& ndc;
	pShader& PixelShade;
	vShader& VertexShade;
	ZBuffer& zBuffer;
private:
	void ProcessVerticies(std::vector<VSIn> vtxes, std::vector<Triangle<int>> triangles)
	{
		std::vector<VSOut> vtxes_out;
		for (auto& v : vtxes) {
			vtxes_out.emplace_back(VertexShade(v));
		}
		ProcessTriangleModel(vtxes_out, triangles);
	}
	void ProcessTriangleModel(std::vector<VSOut>& vtxes, std::vector<Triangle<int>>& triangles)
	{
		for (const auto& t : triangles) {
			const auto& p0 = vtxes[t.v0];
			const auto& p1 = vtxes[t.v1];
			const auto& p2 = vtxes[t.v2];
			if ((((p1.pos - p0.pos) % (p2.pos - p0.pos)).DotProduct(p0.pos) < 0.0f)) {
				ProcessTriangle(Triangle<VSOut>(p0, p1, p2));
			}
		}
	}
	void ProcessTriangle(Triangle<VSOut>& triangle)
	{
		ndc.Transform(triangle.v0);
		ndc.Transform(triangle.v1);
		ndc.Transform(triangle.v2);
		RasterizeTriangle(triangle);
	}
	void RasterizeTriangle(const Triangle<VSOut>& tTriangle)
	{
		DrawTriangle(tTriangle.v0, tTriangle.v1, tTriangle.v2);
	}
	void DrawTriangle(const VSOut& p0, const VSOut& p1, const VSOut& p2)
	{
		const VSOut* pV0 = &p0;
		const VSOut* pV1 = &p1;
		const VSOut* pV2 = &p2;

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
			// Find Splitting VSOut
			const float alpha = (pV1->pos.Y - pV0->pos.Y) / (pV2->pos.Y - pV0->pos.Y);
			const VSOut s_vtx = VSOut(*pV0).InterpolatedTo(*pV2, alpha);

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
	void DrawFlatTopTriangle(const VSOut& p0, const VSOut& p1, const VSOut& p2)
	{
		const float delta_y = p2.pos.Y - p0.pos.Y;
		const VSOut dv0 = (p2 - p0) / delta_y;
		const VSOut dv1 = (p2 - p1) / delta_y;
		VSOut edge1 = p1;
		DrawFlatTriangle(p0, p1, p2, dv0, dv1, edge1);
	}
	void DrawFlatBottomTriangle(const VSOut& p0, const VSOut& p1, const VSOut& p2)
	{
		const float delta_y = p2.pos.Y - p0.pos.Y;
		const VSOut dv0 = (p1 - p0) / delta_y;
		const VSOut dv1 = (p2 - p0) / delta_y;
		VSOut edge1 = p0;
		DrawFlatTriangle(p0, p1, p2, dv0, dv1, edge1);
	}
	void DrawFlatTriangle(const VSOut& p0, const VSOut& p1, const VSOut& p2, const VSOut& dv0, const VSOut& dv1, VSOut edge1)
	{
		VSOut edge0 = p0;
		const int yStart = (int)ceil(p0.pos.Y - 0.5f);
		const int yEnd = (int)ceil(p2.pos.Y - 0.5f);
		edge0 += dv0 * (float(yStart) + 0.5f - p0.pos.Y);
		edge1 += dv1 * (float(yStart) + 0.5f - p0.pos.Y);
		for (int y = yStart; y < yEnd; ++y, edge0 += dv0, edge1 += dv1) {
			const int xStart = (int)ceil(edge0.pos.X - 0.5f);
			const int xEnd = (int)ceil(edge1.pos.X - 0.5f);
			VSOut dvl = (edge1 - edge0) / (edge1.pos.X - edge0.pos.X);
			VSOut ipos = edge0 + dvl * (float(xStart) + 0.5f - edge0.pos.X);
			for (int x = xStart; x < xEnd; ++x, ipos += dvl) {
				if (gfx.ScreenRect().ContainsPoint(iVector2D{ x,y })) {
					const float zCorrection = 1.0f / ipos.pos.Z;
					if (zBuffer.TestAndSet(x, y, zCorrection)) {
						const VSOut pCorrectionVtx = ipos * zCorrection;
						gfx.PutPixel(x, y, PixelShade(pCorrectionVtx));
					}
				}
			}
		}
	}

public:
	Pipeline3D() = delete;
	Pipeline3D(Graphics& gfx, NDCBuffer& ndc, Effect3D& effect3d, ZBuffer& zbuf)
		:
		gfx(gfx),
		ndc(ndc),
		PixelShade(effect3d.PixelShader),
		VertexShade(effect3d.VertexShader),
		zBuffer(zbuf)
	{}
public:
	void ProcessObject3D(const TriangleIndexer<VSIn>& tmodel)
	{
		ProcessVerticies(tmodel.Verticies, tmodel.Triangles);
	}
};
typedef Pipeline3D<tEFFECT_ONLY>	PIPE_tONLY;
typedef Pipeline3D<vbEFFECT_ONLY>	PIPE_vbONLY;
typedef Pipeline3D<mEFFECT_ONLY>	PIPE_mONLY;
typedef Pipeline3D<VB_P2C_EFFECT>	PIPE_VB_P2C;
typedef Pipeline3D<T_SW_EFFECT>		PIPE_T_SW;
typedef Pipeline3D<VB_SW_EFFECT>	PIPE_VB_SW;
typedef Pipeline3D<M_SW_EFFECT>		PIPE_M_SW;

