#pragma once
#include "Sprite.h"
#include "Colors.h"
#include "Vector.h"
#include <functional>

namespace PixelShaders
{
	// 1. textured (default)
	class Texture
	{
	private:
		Sprite texture;
		const int twidth;
		const int theight;

	public:
		class Vertex : public fTextureVector
		{
		public:
			Vertex(fTextureVector tvec)
			{
				pos = tvec.pos;
				tpos = tvec.tpos;
			}
			Vertex(float x, float y, float z, float u, float v)
			{
				pos = { x,y,z };
				tpos = { u,v };
			}
		};
	public:
		Texture(std::string filename)
			:
			texture(filename),
			twidth(texture.getWidth()),
			theight(texture.getHeight())
		{}
		Color operator ()(const Vertex& ipos)
		{
			return texture.GetPixel(int(ipos.tpos.X * twidth) % twidth, int(ipos.tpos.Y * texture.getHeight()) % theight);
		}
	};

	// 2. vertex color blending shader
	class VertexBlend
	{
	public:
		class Vertex
		{
		public:
			fVector3D pos;
			fVector3D color;
		public:
			Vertex(float x, float y, float z, Color color)
				:
				Vertex({ x,y,z }, fVector3D(float(color.GetR()), float(color.GetG()), float(color.GetB())))
			{}
			Vertex(fVector3D vec3, fVector3D color)
				:
				pos(vec3),
				color(color)
			{}
			Vertex operator +(const Vertex& vtx) const
			{
				return Vertex(pos + vtx.pos, color + vtx.color);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, color - vtx.color);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, color * scale);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, color / scale);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& end, const float alpha)
			{
				return Vertex(pos.InterpolatedTo(end.pos, alpha), color.InterpolatedTo(end.color, alpha));
			}
		};
	public:
		Color operator ()(const Vertex& ipos) 
		{
			return Color(ipos.color);
		}
	};

	// 3. monochrome shading
	class Monochrome
	{
	private:
		Color color;

	public:
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
				pos(x,y,z)
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
	public:
		Monochrome(Color color)
			:
			color(color)
		{}
		Color operator ()(const Vertex& ipos)
		{
			return color;
		}
	};
}
typedef PixelShaders::Texture				tPIXELSHADER;
typedef PixelShaders::Texture::Vertex		tpsVERTEX;
typedef PixelShaders::VertexBlend			vbPIXELSHADER;
typedef PixelShaders::VertexBlend::Vertex	vbpsVERTEX;
typedef PixelShaders::Monochrome			mPIXELSHADER;
typedef PixelShaders::Monochrome::Vertex	mpsVERTEX;
