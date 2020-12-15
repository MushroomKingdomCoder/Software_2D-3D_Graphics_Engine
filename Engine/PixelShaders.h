#pragma once
#include "Sprite.h"
#include "Colors.h"
#include "Vector.h"
#include "Effect3D.h"
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
	private:
		bool isNegative;

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
		Texture(std::string filename, bool negative = false)
			:
			texture(filename),
			twidth(texture.getWidth()),
			theight(texture.getHeight()),
			isNegative(negative)
		{}
		Color operator ()(const Vertex& vtx)
		{
			if (!isNegative) {
				return texture.GetPixel(int(vtx.tpos.X * twidth) % twidth, int(vtx.tpos.Y * texture.getHeight()) % theight);
			}
			else {
				return -texture.GetPixel(int(vtx.tpos.X * twidth) % twidth, int(vtx.tpos.Y * texture.getHeight()) % theight);
			}
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
		Color operator ()(const Vertex& vtx) 
		{
			return Color(vtx.color);
		}
	};

	// 3. monochrome shading
	class Monochrome
	{
	private:
		Color color;

	public:
		typedef typename dVertex Vertex;
	public:
		Monochrome(Color color)
			:
			color(color)
		{}
		Color operator ()(const Vertex& vtx)
		{
			return color;
		}
	};

	// 4. Simple color
	class ColoredVertex
	{
	public:
		class Vertex
		{
		public:
			fVector3D pos;
			Color color;
		public:                                                    
			Vertex(dVertex vec3, Color color)
				:
				pos(vec3.pos),
				color(color)
			{}
			Vertex(float x, float y, float z, Color color)
				:
				pos(x, y, z),
				color(color)
			{}
			Vertex operator +(const Vertex& vtx) const
			{
				return Vertex(pos + vtx.pos, color);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, color);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, color);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, color);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& vtx, const float alpha)
			{
				return Vertex(pos.InterpolatedTo(vtx.pos, alpha), color);
			}
		};
	public:
		Color operator ()(const Vertex& vtx)
		{
			return vtx.color;
		}
	};
}
typedef PixelShaders::Texture					tPIXELSHADER;
typedef PixelShaders::Texture::Vertex			tpsVERTEX;
typedef PixelShaders::VertexBlend				vbPIXELSHADER;
typedef PixelShaders::VertexBlend::Vertex		vbpsVERTEX;
typedef PixelShaders::Monochrome				mPIXELSHADER;
typedef PixelShaders::Monochrome::Vertex		mpsVERTEX;
typedef PixelShaders::ColoredVertex				cvPIXELSHADER;
typedef PixelShaders::ColoredVertex::Vertex		cvpsVERTEX;


typedef Effect3D<tPIXELSHADER, EffectDefaults::VertexShader<tpsVERTEX>, EffectDefaults::GeometryShader<tpsVERTEX>>			tEFFECT_ONLY;
typedef Effect3D<vbPIXELSHADER, EffectDefaults::VertexShader<vbpsVERTEX>, EffectDefaults::GeometryShader<vbpsVERTEX>>		vbEFFECT_ONLY;
typedef Effect3D<mPIXELSHADER, EffectDefaults::VertexShader<mpsVERTEX>, EffectDefaults::GeometryShader<mpsVERTEX>>			mEFFECT_ONLY;
typedef Effect3D<cvPIXELSHADER, EffectDefaults::VertexShader<cvpsVERTEX>, EffectDefaults::GeometryShader<cvpsVERTEX>>		cvEFFECT_ONLY;


