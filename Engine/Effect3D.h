#pragma once
#include "Camera3D.h"
#include "Matrix.h"
#include "Triangle.h"
#include "Vector.h"

template <class pShader, class vShader, class gShader>
class Effect3D
{
public:
	typedef typename pShader PIXELSHADER;
	typedef typename vShader VERTEXSHADER;
	typedef typename gShader GEOMETRYSHADER;
public:
	PIXELSHADER PixelShader;
	VERTEXSHADER VertexShader;
	GEOMETRYSHADER GeometryShader;
};


/*

	Default Effect Classes

*/


namespace EffectDefaults
{
	// default vertex class, just a pos vector & a normal
	class Vertex
	{
	public:
		fVector4D pos;
		fVector3D normal;
	public:
		Vertex() = default;
		Vertex(const fVector4D& pos)
			:
			pos(pos),
			normal({0,0,0})
		{}
		Vertex(const fVector4D& pos, const fVector3D& normal)
			:
			pos(pos),
			normal(normal)
		{}
		Vertex(float x, float y, float z)
			:
			pos(x, y, z)
		{}
		Vertex operator +(const Vertex& vtx) const
		{
			return Vertex(pos + vtx.pos, normal);
		}
		Vertex& operator +=(const Vertex& vtx)
		{
			return *this = *this + vtx;
		}
		Vertex operator -(const Vertex& vtx) const
		{
			return Vertex(pos - vtx.pos, normal);
		}
		Vertex& operator -=(const Vertex& vtx)
		{
			return *this = *this - vtx;
		}
		Vertex operator *(const float scale) const
		{
			return Vertex(pos * scale, normal);
		}
		Vertex& operator *=(const float scale)
		{
			return *this = *this * scale;
		}
		Vertex operator /(const float scale) const
		{
			return Vertex(pos / scale, normal);
		}
		Vertex& operator /=(const float scale)
		{
			return *this = *this / scale;
		}
		Vertex InterpolatedTo(const Vertex& vtx, const float alpha) const
		{
			return Vertex(pos.InterpolatedTo(vtx.pos, alpha), normal);
		}
		bool operator ==(const Vertex& vtx) const
		{
			return (pos == vtx.pos && normal == vtx.normal);
		}
		bool operator !=(const Vertex& vtx) const
		{
			return !(*this == vtx);
		}
	};

	// default vertex shader, no special effects
	template <class vertex>
	class VertexShader
	{
	private:
		fMatrix3Dplus ObjectTransformation = fMatrix3Dplus::Identity();
		fMatrix3Dplus CameraTransformation = fMatrix3Dplus::Identity();
		fMatrix3Dplus WorldTransformation = fMatrix3Dplus::Identity();
		fMatrix3Dplus Projection = fMatrix3Dplus::Projection(2, 2, 1, 100);
		fMatrix3Dplus ScreenTransformation = Projection * WorldTransformation;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		VertexShader(const fMatrix3Dplus& o_transform, const fMatrix3Dplus& proj, const fMatrix3Dplus& c_transform = fMatrix3Dplus::Identity())
			:
			ObjectTransformation(o_transform),
			CameraTransformation(c_transform),
			WorldTransformation(CameraTransformation * ObjectTransformation),
			Projection(proj),
			ScreenTransformation(Projection * WorldTransformation)
		{}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.pos = ScreenTransformation * vtx_in.pos;
			vtx_in.normal = WorldTransformation * fVector4D(vtx_in.normal, 0.0f); 
			return vtx_in;
		}
		void SetProjectionMatrix(const fMatrix3Dplus& projection)
		{
			Projection = projection;
			ScreenTransformation = Projection * WorldTransformation;
		}
		void SetObjectTransformationMatrix(const fMatrix3Dplus& o_transform)
		{
			ObjectTransformation = o_transform;
			WorldTransformation = CameraTransformation * ObjectTransformation;
			ScreenTransformation = Projection * WorldTransformation;
		}
		void SetCameraTransformation(const fMatrix3Dplus& c_transform)
		{
			CameraTransformation = c_transform;
			WorldTransformation = CameraTransformation * ObjectTransformation;
			ScreenTransformation = Projection * WorldTransformation;
		}
		const fMatrix3Dplus& GetProjectionMatrix() const
		{
			return Projection;
		}
 	};

	// default vertex shader, no special effects (assumes per-pixel lighted verticies w/World_Pos storage)
	template <class vertex>
	class VertexShader_PPS
	{
	private:
		fMatrix3Dplus ObjectTransformation = fMatrix3Dplus::Identity();
		fMatrix3Dplus CameraTransformation = fMatrix3Dplus::Identity();
		fMatrix3Dplus WorldTransformation = fMatrix3Dplus::Identity();
		fMatrix3Dplus Projection = fMatrix3Dplus::Projection(2, 2, 1, 100);
		fMatrix3Dplus ScreenTransformation = Projection * WorldTransformation;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		VertexShader_PPS(const fMatrix3Dplus& o_transform, const fMatrix3Dplus& proj, const fMatrix3Dplus& c_transform = fMatrix3Dplus::Identity())
			:
			ObjectTransformation(o_transform),
			CameraTransformation(c_transform),
			WorldTransformation(CameraTransformation * WorldTransformation),
			Projection(proj),
			ScreenTransformation(Projection * WorldTransformation)
		{}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.World_Pos = WorldTransformation * vtx_in.pos;
			vtx_in.pos = ScreenTransformation * vtx_in.pos;
			vtx_in.normal = WorldTransformation * fVector4D(vtx_in.normal, 0.0f);
			return vtx_in;
		}
		void SetProjectionMatrix(const fMatrix3Dplus& projection)
		{
			Projection = projection;
			ScreenTransformation = Projection * WorldTransformation;
		}
		void SetObjectTransformationMatrix(const fMatrix3Dplus& o_transform)
		{
			ObjectTransformation = o_transform;
			WorldTransformation = CameraTransformation * ObjectTransformation;
			ScreenTransformation = Projection * WorldTransformation;
		}
		void SetCameraTransformation(const fMatrix3Dplus& c_transform)
		{
			CameraTransformation = c_transform;
			WorldTransformation = CameraTransformation * ObjectTransformation;
			ScreenTransformation = Projection * WorldTransformation;
		}
		const fMatrix3Dplus& GetProjectionMatrix() const
		{
			return Projection;
		}
	};

	// default geometry shader, exports triangle of verticies
	template <class vertex>
	class GeometryShader
	{
	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		Triangle<VertexOut> operator ()(const VertexIn& v0, const VertexIn& v1, const VertexIn& v2, int id)
		{
			return Triangle<VertexOut>(VertexOut(v0), VertexOut(v1), VertexOut(v2));
		}
	};
}
typedef EffectDefaults::Vertex dVertex;
