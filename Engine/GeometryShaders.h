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
				VertexOut(v0.pos, colors[int(id / 2)], v0.normal),
				VertexOut(v1.pos, colors[int(id / 2)], v1.normal),
				VertexOut(v2.pos, colors[int(id / 2)], v2.normal));
		}
	};

	// 2. directional lighting (vertex flat or goraud)
	template <typename vertex>
	class DirectionalLighting
	{
	private:
		const bool isGoraud = true;
		fVector3D lighting = { 1,1,1 };
		fVector3D ambience = { 0.1f,0.1f,0.1f };
		fVector3D direction = { 0,0,1 };

	public:
		DirectionalLighting() = default;
		DirectionalLighting(bool isGoraud)
			:
			isGoraud(isGoraud)
		{}
		DirectionalLighting(fVector3D lighting, fVector3D ambience, fVector3D direction, bool isGoraud = true)
			:
			lighting(lighting),
			ambience(ambience),
			direction(direction),
			isGoraud(isGoraud)
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
		Triangle<VertexOut> operator ()(VertexIn v0, VertexIn v1, VertexIn v2, int id) const
		{
			if (isGoraud) {
				const fVector3D v0n = v0.normal.Normalized(); const fVector3D v1n = v1.normal.Normalized(); const fVector3D v2n = v2.normal.Normalized();
				const fVector3D v0light = fVector3D((lighting * direction.DotProduct(v0n)).Saturated() + ambience).Saturated();
				const fVector3D v1light = fVector3D((lighting * direction.DotProduct(v1n)).Saturated() + ambience).Saturated();
				const fVector3D v2light = fVector3D((lighting * direction.DotProduct(v2n)).Saturated() + ambience).Saturated();
				v0.light = v0light; v1.light = v1light; v2.light = v2light;
				return Triangle<VertexOut>(v0, v1, v2);
			}
			else {
				const fVector3D face_normal = fVector3D((v1.pos - v0.pos) % (v2.pos - v0.pos)).Normalized();
				fVector3D light = fVector3D((-lighting * direction.DotProduct(face_normal)).Saturated() + ambience).Saturated();
				v0.light = light; v1.light = light; v2.light = light;
				return Triangle<VertexOut>(v0, v1, v2);
			}
		}
	};

	// 3. point lighting (vertex flat or goraud)
	template <typename vertex>
	class PointLighting
	{
	private:
		const bool isGoraud = true;
		fVector3D lighting = { 1,1,1 };
		fVector3D ambience = { 0.1f,0.1f,0.1f };
		fVector3D Position = { 0,0,1 };
		fMatrix3Dplus Projection = fMatrix3Dplus::Identity();
		fVector3D position = Projection * position;
		const float constant_attenuation = 0.4f;
		const float linear_attenuation = 0.1f;
		const float quadratic_attenuation = 0.5f;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut; 
	public:
		PointLighting() = default;
		PointLighting(bool isGoraud)
			:
			isGoraud(isGoraud)
		{}
		PointLighting(fVector3D pos, bool isGoraud = true)
			:
			position(pos),
			isGoraud(isGoraud)
		{}
		PointLighting(float ca, float la, float qa, bool isGoraud = true)
			:
			constant_attenuation(ca),
			linear_attenuation(la),
			quadratic_attenuation(qa),
			isGoraud(isGoraud)
		{}
		PointLighting(fVector3D l, fVector3D a, fVector3D pos, bool isGoraud = true)
			:
			lighting(l),
			ambience(a),
			position(pos),
			isGoraud(isGoraud)
		{}
		PointLighting(fVector3D l, fVector3D a, fVector3D pos, float ca, float la, float qa, bool isGoraud = true)
			:
			lighting(l),
			ambience(a),
			position(pos),
			constant_attenuation(ca),
			linear_attenuation(la),
			quadratic_attenuation(qa),
			isGoraud(isGoraud)
		{}
		void Move(const fVector3D& translation)
		{
			Position += translation;
			position = Projection * Position;
		}
		void SetPosition(const fVector3D& pos)
		{
			Position = pos;
			position = Projection * Position;
		}
		const fVector3D& GetPosition() const
		{
			return Position;
		}
		void SetProjectionMatrix(const fMatrix3Dplus& proj)
		{
			Projection = proj;
			position = Projection * Position;
		}
	public:
		Triangle<VertexOut> operator ()(VertexIn v0, VertexIn v1, VertexIn v2, int id) const
		{
			if (isGoraud) {
				const fVector3D v0n = v0.normal.Normalized(); const fVector3D v1n = v1.normal.Normalized(); const fVector3D v2n = v2.normal.Normalized();
				const fVector3D v0DD = position - v0.pos; const fVector3D v1DD = position - v1.pos; const fVector3D v2DD = position - v2.pos;
				const float v0d = v0DD.Length(); const float v1d = v1DD.Length(); const float v2d = v2DD.Length();
				const float v0att = 1 / (quadratic_attenuation * sq(v0d) + linear_attenuation * v0d + constant_attenuation);
				const float v1att = 1 / (quadratic_attenuation * sq(v1d) + linear_attenuation * v1d + constant_attenuation);
				const float v2att = 1 / (quadratic_attenuation * sq(v2d) + linear_attenuation * v2d + constant_attenuation);
				const fVector3D v0LN = v0DD.Normalized(); const fVector3D v1LN = v1DD.Normalized(); const fVector3D v2LN = v2DD.Normalized();
				const fVector3D v0light = ((-lighting * v0att * v0LN.DotProduct(v0n)).Saturated() + ambience).Saturated();
				const fVector3D v1light = ((-lighting * v1att * v1LN.DotProduct(v1n)).Saturated() + ambience).Saturated();
				const fVector3D v2light = ((-lighting * v2att * v2LN.DotProduct(v2n)).Saturated() + ambience).Saturated();
				v0.light = v0light; v1.light = v1light; v2.light = v2light;
				return Triangle<VertexOut>(v0, v1, v2);
			}
			else {
				const fVector3D face_normal = fVector3D((v1.pos - v0.pos) % (v2.pos - v0.pos)).Normalized();
				fVector3D midpoint = (v1.pos + v0.pos) / 2.0f; midpoint = (v2.pos + midpoint) / 2.0f;
				const float distance = (position - midpoint).Length();
				const float attenuation = 1 / (quadratic_attenuation * sq(distance) + linear_attenuation * distance + constant_attenuation);
				const fVector3D ldirection = (position - midpoint).Normalized();
				const fVector3D light = ((lighting * attenuation * ldirection.DotProduct(face_normal)).Saturated() + ambience).Saturated();
				v0.light = light; v1.light = light; v2.light = light;
				return Triangle<VertexOut>(v0, v1, v2);
			}
		}
	};
}
typedef GeometryShaders::QuadColoring								qcGEOMETRYSHADER;
typedef GeometryShaders::DirectionalLighting<ltpsVERTEX>			tdlGEOMETRYSHADER;
typedef GeometryShaders::DirectionalLighting<lvbpsVERTEX>			vbdlGEOMETRYSHADER;
typedef GeometryShaders::DirectionalLighting<lmpsVERTEX>			mdlGEOMETRYSHADER;
typedef GeometryShaders::DirectionalLighting<lcvpsVERTEX>			cvdlGEOMETRYSHADER;
typedef GeometryShaders::PointLighting<ltpsVERTEX>					tplGEOMETRYSHADER;
typedef GeometryShaders::PointLighting<lvbpsVERTEX>					vbplGEOMETRYSHADER;
typedef GeometryShaders::PointLighting<lmpsVERTEX>					mplGEOMETRYSHADER;
typedef GeometryShaders::PointLighting<lcvpsVERTEX>					cvplGEOMETRYSHADER;


// ***Quad Coloring***
typedef Effect3D<cvPIXELSHADER, dmVERTEXSHADER, qcGEOMETRYSHADER>			CV_D_QC_EFFECT;
// ***Diectional Lighting***
typedef Effect3D<ltPIXELSHADER, dtVERTEXSHADER, tdlGEOMETRYSHADER>			LT_D_DL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, dvbVERTEXSHADER, vbdlGEOMETRYSHADER>		LVB_D_DL_EFFECT;
typedef Effect3D<lmPIXELSHADER, dmVERTEXSHADER, mdlGEOMETRYSHADER>			LM_D_DL_EFFECT;
typedef Effect3D<lcvPIXELSHADER, dcvVERTEXSHADER, cvdlGEOMETRYSHADER>		LCV_D_DL_EFFECT;
typedef Effect3D<ltPIXELSHADER, swtVERTEXSHADER, tdlGEOMETRYSHADER>			LT_SW_DL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, swvbVERTEXSHADER, vbdlGEOMETRYSHADER>		LVB_SW_DL_EFFECT;
typedef Effect3D<lmPIXELSHADER, swmVERTEXSHADER, mdlGEOMETRYSHADER>			LM_SW_DL_EFFECT;
typedef Effect3D<lcvPIXELSHADER, swcvVERTEXSHADER, cvdlGEOMETRYSHADER>		LCV_SW_DL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, pcVERTEXSHADER, vbdlGEOMETRYSHADER>		LVB_P2C_DL_EFFECT;
// ***Point Lighting***
typedef Effect3D<ltPIXELSHADER, dtVERTEXSHADER, tplGEOMETRYSHADER>			LT_D_PL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, dvbVERTEXSHADER, vbplGEOMETRYSHADER>		LVB_D_PL_EFFECT;
typedef Effect3D<lmPIXELSHADER, dmVERTEXSHADER, mplGEOMETRYSHADER>			LM_D_PL_EFFECT;
typedef Effect3D<lcvPIXELSHADER, dcvVERTEXSHADER, cvplGEOMETRYSHADER>		LCV_D_PL_EFFECT;
typedef Effect3D<ltPIXELSHADER, swtVERTEXSHADER, tplGEOMETRYSHADER>			LT_SW_PL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, swvbVERTEXSHADER, vbplGEOMETRYSHADER>		LVB_SW_PL_EFFECT;
typedef Effect3D<lmPIXELSHADER, swmVERTEXSHADER, mplGEOMETRYSHADER>			LM_SW_PL_EFFECT;
typedef Effect3D<lcvPIXELSHADER, swcvVERTEXSHADER, cvplGEOMETRYSHADER>		LCV_SW_PL_EFFECT;
typedef Effect3D<lvbPIXELSHADER, pcVERTEXSHADER, vbplGEOMETRYSHADER>		LVB_P2C_PL_EFFECT;
