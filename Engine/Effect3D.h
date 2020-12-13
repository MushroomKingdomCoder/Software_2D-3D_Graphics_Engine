#pragma once
#include "Matrix.h"
#include "Vector.h"

template <class pShader, class vShader>
class Effect3D
{
public:
	typedef typename pShader PIXELSHADER;
	typedef typename vShader VERTEXSHADER;
public:
	PIXELSHADER PixelShader;
	VERTEXSHADER VertexShader;
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
	public:
		Vertex(fVector3D vec3)
			:
			pos(vec3)
		{}
		Vertex(float x, float y, float z)
			:
			pos(x, y, z)
		{}
		Vertex operator +(const Vertex& vtx) const
		{
			return Vertex(pos + vtx.pos);
		}
		Vertex& operator +=(const Vertex& vtx)
		{
			return *this = *this + vtx;
		}
		Vertex operator -(const Vertex& vtx) const
		{
			return Vertex(pos - vtx.pos);
		}
		Vertex& operator -=(const Vertex& vtx)
		{
			return *this = *this - vtx;
		}
		Vertex operator *(const float scale) const
		{
			return Vertex(pos * scale);
		}
		Vertex& operator *=(const float scale)
		{
			return *this = *this * scale;
		}
		Vertex operator /(const float scale) const
		{
			return Vertex(pos / scale);
		}
		Vertex& operator /=(const float scale)
		{
			return *this = *this / scale;
		}
		Vertex InterpolatedTo(const Vertex& vtx, const float alpha)
		{
			return Vertex(pos.InterpolatedTo(vtx.pos, alpha));
		}
	};

	// default vertex shader, no special effects
	template <class pShader>
	class VertexShader
	{
	private:
		fMatrix3D const* pRotation;
		fVector3D const* pTranslation;

	public:
		typedef typename pShader::Vertex VertexIn;
		typedef typename VertexIn VertexOut;
	public:
		VertexShader(const fMatrix3D& rotation, const fVector3D& translation)
			:
			pRotation(&rotation),
			pTranslation(&translation)
		{}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.pos = *pRotation * vtx_in.pos;
			vtx_in.pos += *pTranslation;
			return VertexOut(vtx_in);
		}
		void BindRotation(const fMatrix3D& rotation)
		{
			pRotation = &rotation;
		}
		void BindTranslation(const fVector3D translation)
		{
			pTranslation = &translation;
		}
 	};
}
typedef EffectDefaults::Vertex dVertex;
