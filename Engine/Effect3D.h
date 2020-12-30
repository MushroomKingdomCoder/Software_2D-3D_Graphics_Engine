#pragma once
#include "Triangle.h"
#include "Matrix.h"
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
	// default vertex class, just a pos vector
	class Vertex
	{
	public:
		fVector3D pos;
		fVector3D normal;
	public:
		Vertex() = default;
		Vertex(const fVector3D& pos)
			:
			pos(pos),
			normal({0,0,0})
		{}
		Vertex(const fVector3D& pos, const fVector3D& normal)
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
		Vertex InterpolatedTo(const Vertex& vtx, const float alpha)
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
		fMatrix3Dplus const* pTransformation;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		VertexShader(const fMatrix3Dplus& transformation)
			:
			pTransformation(&transformation)
		{}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.pos = *pTransformation * vtx_in.pos;
			vtx_in.normal = *pTransformation * fVector4D(vtx_in.normal, 0.0f); 
			return VertexOut(vtx_in);
		}
 	};

	// default vertex shader, no special effects (assumes per-pixel lighted verticies)
	template <class vertex>
	class VertexShader_PPS
	{
	private:
		fMatrix3Dplus const* pTransformation;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		VertexShader_PPS(const fMatrix3Dplus& transformation)
			:
			pTransformation(&transformation)
		{}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.pos = *pTransformation * vtx_in.pos;
			vtx_in.normal = *pTransformation * fVector4D(vtx_in.normal, 0.0f);
			vtx_in.World_Pos = vtx_in.pos;
			return VertexOut(vtx_in);
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
