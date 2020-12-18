#pragma once
#include "Effect3D.h"
#include "VertexShaders.h"
#include "PixelShaders.h"
#include "Colors.h"
#include "Triangle.h"
#include "Vector.h"

namespace GeometryShaders
{
	// 1. Colors surfaces of a concave shape as pairs of triangles
	class QuadColoring
	{
	private:
		std::vector<Color> colors;

	public:
		typedef typename dVertex VertexIn;
		typedef typename cvpsVERTEX VertexOut;
	public:
		QuadColoring(std::vector<Color> colors)
			:
			colors(colors)
		{}
		Triangle<VertexOut> operator ()(const VertexIn& v0, const VertexIn& v1, const VertexIn& v2, int id)
		{
			return Triangle<VertexOut>(
				VertexOut(v0, colors[int(id / 2)]),
				VertexOut(v1, colors[int(id / 2)]),
				VertexOut(v2, colors[int(id / 2)]));
		}
	};

	// 2. vertex flat directional lighting
	template <typename vertex>
	class DirectionalLighting
	{
	private:
		fVector3D lighting = { 1,1,1 };
		fVector3D ambience = { 0.05f,0.05f,0.05f };
		fVector3D direction = { 0,0,1 };

	public:
		DirectionalLighting() = default;
		DirectionalLighting(fVector3D lighting, fVector3D ambience, fVector3D direction)
			:
			lighting(lighting),
			ambience(ambience),
			direction(direction)
		{}
		void RotateLightX(const float radians)
		{
			direction = fMatrix3D::RotationX(radians) * direction;
		}
		void RotateLightY(const float radians)
		{
			direction = fMatrix3D::RotationY(radians) * direction;
		}
		void RotateLightZ(const float radians)
		{
			direction = fMatrix3D::RotationZ(radians) * direction;
		}
	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		Triangle<VertexOut> operator ()(VertexIn v0, VertexIn v1, VertexIn v2, int id)
		{
			const fVector3D face_normal = fVector3D((v1.pos - v0.pos) % (v2.pos - v0.pos)).Normalized();
			fVector3D light = fVector3D((-lighting * direction.DotProduct(face_normal)).Saturated() + ambience).Saturated();
			v0.light = light; v1.light = light; v2.light = light;
			return Triangle<VertexOut>(v0, v1, v2);
		}
	};
}
typedef GeometryShaders::QuadColoring								qcGEOMETRYSHADER;
typedef GeometryShaders::DirectionalLighting<ltpsVERTEX>			tdlGEOMETRYSHADER;
typedef GeometryShaders::DirectionalLighting<lvbpsVERTEX>			vbdlGEOMETRYSHADER;
typedef GeometryShaders::DirectionalLighting<lmpsVERTEX>			mdlGEOMETRYSHADER;
typedef GeometryShaders::DirectionalLighting<lcvpsVERTEX>			cvdlGEOMETRYSHADER;


typedef Effect3D<cvPIXELSHADER, dmVERTEXSHADER, qcGEOMETRYSHADER>			CV_D_QC_EFFECT;
typedef Effect3D<ltPIXELSHADER, dtVERTEXSHADER, tdlGEOMETRYSHADER>			LT_D_DL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, dvbVERTEXSHADER, vbdlGEOMETRYSHADER>		LVB_D_DL_EFFECT;
typedef Effect3D<lmPIXELSHADER, dmVERTEXSHADER, mdlGEOMETRYSHADER>			LM_D_DL_EFFECT;
typedef Effect3D<lcvPIXELSHADER, dcvVERTEXSHADER, cvdlGEOMETRYSHADER>		LCV_D_DL_EFFECT;
typedef Effect3D<ltPIXELSHADER, swtVERTEXSHADER, tdlGEOMETRYSHADER>			LT_SW_DL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, swvbVERTEXSHADER, vbdlGEOMETRYSHADER>		LVB_SW_DL_EFFECT;
typedef Effect3D<lmPIXELSHADER, swmVERTEXSHADER, mdlGEOMETRYSHADER>			LM_SW_DL_EFFECT;
typedef Effect3D<lcvPIXELSHADER, swcvVERTEXSHADER, cvdlGEOMETRYSHADER>		LCV_SW_DL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, pcVERTEXSHADER, vbdlGEOMETRYSHADER>		LVB_P2C_DL_EFFECT;
