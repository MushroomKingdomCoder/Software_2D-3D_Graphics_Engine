#pragma once
#include "Graphics.h"
#include "Sprite.h"
#include "NDCBuffer.h"
#include "Indexer3D.h"
#include "PixelShaders.h"
#include "VertexShaders.h"
#include "GeometryShaders.h"
#include "ZBuffer.h"
#include "Matrix.h"
#include "Vector.h"

template <class Effect3D>
class Pipeline3D
{
	typedef typename Effect3D::VERTEXSHADER::VertexIn Vertex;
	typedef typename Effect3D::VERTEXSHADER::VertexOut VSOut;
	typedef typename Effect3D::GEOMETRYSHADER::VertexOut GSOut;
private:
	Graphics& gfx;
	NDCBuffer& ndc;
	Effect3D& Effect;
	ZBuffer& zBuffer;
private:
	const fVector4D eye_pos = Effect.VertexShader.GetProjectionMatrix() * fVector4D(0, 0, 0, 1);
private:
	void ProcessVerticies(std::vector<Vertex> vtxes, std::vector<Triangle<size_t>> triangles)
	{
		std::vector<VSOut> vtxes_out;
		for (auto& v : vtxes) {
			vtxes_out.emplace_back(Effect.VertexShader(v));
		}
		ProcessTriangleModel(vtxes_out, triangles);
	}
	void ProcessTriangleModel(std::vector<VSOut>& vtxes, std::vector<Triangle<size_t>>& triangles)
	{
		int id = 0;
		for (const auto& t : triangles) {
			const auto& p0 = vtxes[t.v0];
			const auto& p1 = vtxes[t.v1];
			const auto& p2 = vtxes[t.v2];
			if (((fVector3D(p1.pos - p0.pos) % fVector3D(p2.pos - p0.pos)).DotProduct(fVector3D(p0.pos - eye_pos)) < 0.0f)) {
				ProcessTriangle(Effect.GeometryShader(p0, p1, p2, id));
			}
			++id;
		}
	}
	void ProcessTriangle(Triangle<GSOut>& triangle)
	{
		ndc.Transform(triangle.v0);
		ndc.Transform(triangle.v1);
		ndc.Transform(triangle.v2);
		RasterizeTriangle(triangle);
	}
	void RasterizeTriangle(const Triangle<GSOut>& tTriangle)
	{
		DrawTriangle(tTriangle.v0, tTriangle.v1, tTriangle.v2);
	}
	void DrawTriangle(const GSOut& p0, const GSOut& p1, const GSOut& p2)
	{
		const GSOut* pV0 = &p0;
		const GSOut* pV1 = &p1;
		const GSOut* pV2 = &p2;

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
			// Find Splitting GSOut
			const float alpha = (pV1->pos.Y - pV0->pos.Y) / (pV2->pos.Y - pV0->pos.Y);
			const GSOut s_vtx = GSOut(*pV0).InterpolatedTo(*pV2, alpha);

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
	void DrawFlatTopTriangle(const GSOut& p0, const GSOut& p1, const GSOut& p2)
	{
		const float delta_y = p2.pos.Y - p0.pos.Y;
		const GSOut dv0 = (p2 - p0) / delta_y;
		const GSOut dv1 = (p2 - p1) / delta_y;
		GSOut edge1 = p1;
		DrawFlatTriangle(p0, p1, p2, dv0, dv1, edge1);
	}
	void DrawFlatBottomTriangle(const GSOut& p0, const GSOut& p1, const GSOut& p2)
	{
		const float delta_y = p2.pos.Y - p0.pos.Y;
		const GSOut dv0 = (p1 - p0) / delta_y;
		const GSOut dv1 = (p2 - p0) / delta_y;
		GSOut edge1 = p0;
		DrawFlatTriangle(p0, p1, p2, dv0, dv1, edge1);
	}
	void DrawFlatTriangle(const GSOut& p0, const GSOut& p1, const GSOut& p2, const GSOut& dv0, const GSOut& dv1, GSOut edge1)
	{
		GSOut edge0 = p0;
		const int yStart = (int)ceil(p0.pos.Y - 0.5f);
		const int yEnd = (int)ceil(p2.pos.Y - 0.5f);
		edge0 += dv0 * (float(yStart) + 0.5f - p0.pos.Y);
		edge1 += dv1 * (float(yStart) + 0.5f - p0.pos.Y);
		for (int y = yStart; y < yEnd; ++y, edge0 += dv0, edge1 += dv1) {
			const int xStart = (int)ceil(edge0.pos.X - 0.5f);
			const int xEnd = (int)ceil(edge1.pos.X - 0.5f);
			GSOut dvl = (edge1 - edge0) / (edge1.pos.X - edge0.pos.X);
			GSOut ipos = edge0 + dvl * (float(xStart) + 0.5f - edge0.pos.X);
			for (int x = xStart; x < xEnd; ++x, ipos += dvl) {
				if (gfx.ScreenRect().ContainsPoint(iVector2D{ x,y })) {
					if (zBuffer.TestAndSet(x, y, ipos.pos.Z)) {
						const float zCorrection = 1.0f / ipos.pos.W;
						const GSOut pCorrectionVtx = ipos * zCorrection;
						gfx.PutPixel(x, y, Effect.PixelShader(pCorrectionVtx));
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
		Effect(effect3d),
		zBuffer(zbuf),
		eye_pos(Effect.VertexShader.GetProjectionMatrix() * fVector4D(0, 0, 0, 1))
	{}
public:
	void ProcessMesh(const TriangleIndexer<Vertex>& tmodel)
	{
		ProcessVerticies(tmodel.Verticies, tmodel.Triangles);
	}
};
// ***Pixel Shaders Only***
typedef Pipeline3D<tEFFECT_ONLY>		PIPE_tONLY;
typedef Pipeline3D<vbEFFECT_ONLY>		PIPE_vbONLY;
typedef Pipeline3D<mEFFECT_ONLY>		PIPE_mONLY;
typedef Pipeline3D<cvEFFECT_ONLY>		PIPE_cvONLY;
// ***Pixel & Vertex Shaders***
typedef Pipeline3D<VB_P2C_EFFECT>		PIPE_VB_P2C;
typedef Pipeline3D<T_SW_EFFECT>			PIPE_T_SW;
typedef Pipeline3D<VB_SW_EFFECT>		PIPE_VB_SW;
typedef Pipeline3D<M_SW_EFFECT>			PIPE_M_SW;
typedef Pipeline3D<CV_SW_EFFECT>		PIPE_CV_SW;
// ***Pixel, Vertex, & Geometry Shaders***
typedef Pipeline3D<CV_D_QC_EFFECT>		PIPE_CV_D_QC;
		// ***Directional Lighting***
typedef Pipeline3D<LT_D_DL_EFFECT>		PIPE_LT_D_DL;
typedef Pipeline3D<LVB_D_DL_EFFECT>		PIPE_LVB_D_DL;
typedef Pipeline3D<LM_D_DL_EFFECT>		PIPE_LM_D_DL;
typedef Pipeline3D<LCV_D_DL_EFFECT>		PIPE_LCV_D_DL;
typedef Pipeline3D<LT_SW_DL_EFFECT>		PIPE_LT_SW_DL;
typedef Pipeline3D<LVB_SW_DL_EFFECT>	PIPE_LVB_SW_DL;
typedef Pipeline3D<LM_SW_DL_EFFECT>		PIPE_LM_SW_DL;
typedef Pipeline3D<LCV_SW_DL_EFFECT>	PIPE_LCV_SW_DL;
typedef Pipeline3D<LVB_P2C_DL_EFFECT>	PIPE_LVB_P2C_DL;
		// ***Point Lighting***
typedef Pipeline3D<LT_D_PL_EFFECT>		PIPE_LT_D_PL;
typedef Pipeline3D<LVB_D_PL_EFFECT>		PIPE_LVB_D_PL;
typedef Pipeline3D<LM_D_PL_EFFECT>		PIPE_LM_D_PL;
typedef Pipeline3D<LCV_D_PL_EFFECT>		PIPE_LCV_D_PL;
typedef Pipeline3D<LT_SW_PL_EFFECT>		PIPE_LT_SW_PL;
typedef Pipeline3D<LVB_SW_PL_EFFECT>	PIPE_LVB_SW_PL;
typedef Pipeline3D<LM_SW_PL_EFFECT>		PIPE_LM_SW_PL;
typedef Pipeline3D<LCV_SW_PL_EFFECT>	PIPE_LCV_SW_PL;
typedef Pipeline3D<LVB_P2C_PL_EFFECT>	PIPE_LVB_P2C_PL;
		// ***Per-Pixel Lighting***
typedef Pipeline3D<pptEFFECT_ONLY>		PIPE_pptONLY;
typedef Pipeline3D<PPT_SW_EFFECT>		PIPE_PPT_SW;
typedef Pipeline3D<ppmEFFECT_ONLY>		PIPE_ppmONLY; 
typedef Pipeline3D<PPM_SW_EFFECT>		PIPE_PPM_SW;


