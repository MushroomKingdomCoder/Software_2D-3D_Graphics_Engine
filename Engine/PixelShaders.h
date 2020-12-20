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
		class Vertex
		{
		public:
			fVector3D pos;
			fVector2D tpos;
		public:
			Vertex() = default;
			Vertex(float x, float y, float z, float tx, float ty)
				:
				pos(x, y, z),
				tpos(tx, ty)
			{}
			Vertex(fVector3D pos, fVector2D tpos)
				:
				pos(pos),
				tpos(tpos)
			{}
			Vertex operator +(const Vertex & tvec) const
			{
				return Vertex(pos + tvec.pos, tpos + tvec.tpos);
			}
			Vertex& operator +=(const Vertex & tvec)
			{
				return *this = *this + tvec;
			}
			Vertex operator -(const Vertex & tvec) const
			{
				return Vertex(pos - tvec.pos, tpos - tvec.tpos);
			}
			Vertex& operator -=(const Vertex & tvec)
			{
				return *this = *this - tvec;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, tpos * scale);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, tpos / scale);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex & end, const float alpha) const
			{
				return Vertex(
					fVector3D(pos).InterpolatedTo(end.pos, alpha),
					fVector2D(tpos).InterpolatedTo(end.tpos, alpha)
				);
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
			Color color = texture.GetPixel(int(vtx.tpos.X * twidth) % twidth, int(vtx.tpos.Y * texture.getHeight()) % theight);
			if (!isNegative) {
				return color;
			}
			else {
				return -color;
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
			Vertex(fVector3D vec3)
				:
				pos(vec3)
			{}
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
			return vtx.color;
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

	/*
	
		***LIGHTED EFFECTS***
	
	*/

	// 1. textured (lighted)
	class lTexture
	{
	private:
		Sprite texture;
		const int twidth;
		const int theight;
	private:
		bool isNegative;

	public:
		class Vertex
		{
		public:
			fVector3D pos;
			fVector2D tpos;
			fVector3D light;
		public:
			Vertex() = default;
			Vertex(const Texture::Vertex& vtx)
				:
				pos(vtx.pos),
				tpos(vtx.tpos)
			{}
			Vertex(float x, float y, float z, float tx, float ty, fVector3D light)
				:
				pos(x, y, z),
				tpos(tx, ty),
				light(light)
			{}
			Vertex(fVector3D pos, fVector2D tpos, fVector3D light)
				:
				pos(pos),
				tpos(tpos),
				light(light)
			{}
			Vertex operator +(const Vertex & tvec) const
			{
				return Vertex(pos + tvec.pos, tpos + tvec.tpos, light + tvec.light);
			}
			Vertex& operator +=(const Vertex & tvec)
			{
				return *this = *this + tvec;
			}
			Vertex operator -(const Vertex & tvec) const
			{
				return Vertex(pos - tvec.pos, tpos - tvec.tpos, light - tvec.light);
			}
			Vertex& operator -=(const Vertex & tvec)
			{
				return *this = *this - tvec;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, tpos * scale, light * scale);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, tpos / scale, light / scale);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex & end, const float alpha) const
			{
				return Vertex(
					fVector3D(pos).InterpolatedTo(end.pos, alpha),
					fVector2D(tpos).InterpolatedTo(end.tpos, alpha), 
					fVector3D(light).InterpolatedTo(end.light, alpha)
				);
			}
		};
	public:
		lTexture(std::string filename, bool negative = false)
			:
			texture(filename),
			twidth(texture.getWidth()),
			theight(texture.getHeight()),
			isNegative(negative)
		{}
		Color operator ()(const Vertex& vtx)
		{
			Color color = texture.GetPixel(int(vtx.tpos.X * twidth) % twidth, int(vtx.tpos.Y * texture.getHeight()) % theight);
			Color lighted_color = fVector3D(color.GetR(), color.GetG(), color.GetB()).GetHadamardProduct(vtx.light);
			if (!isNegative) {
				return lighted_color;
			}
			else {
				return -lighted_color;
			}
		}
	};

	// 2. vertex color blending shader (lighted)
	class lVertexBlend
	{
	public:
		class Vertex
		{
		public:
			fVector3D pos;
			fVector3D color;
			fVector3D light;
		public:
			Vertex() = default;
			Vertex(const VertexBlend::Vertex& vtx)
				:
				pos(vtx.pos),
				color(vtx.color)
			{}
			Vertex(float x, float y, float z, Color color, fVector3D light)
				:
				Vertex({ x,y,z }, fVector3D(float(color.GetR()), float(color.GetG()), float(color.GetB())), light)
			{}
			Vertex(fVector3D vec3, fVector3D color, fVector3D light)
				:
				pos(vec3),
				color(color),
				light(light)
			{}
			Vertex operator +(const Vertex& vtx) const
			{
				return Vertex(pos + vtx.pos, color + vtx.color, light + vtx.light);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, color - vtx.color, light - vtx.light);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, color * scale, light * scale);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, color / scale, light / scale);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& end, const float alpha)
			{
				return Vertex(
					pos.InterpolatedTo(end.pos, alpha), 
					color.InterpolatedTo(end.color, alpha), 
					light.InterpolatedTo(end.light, alpha));
			}
		};
	public:
		Color operator ()(const Vertex& vtx)
		{
			return Color(vtx.color.GetHadamardProduct(vtx.light));
		}
	};

	// 3. monochrome shading (lighted)
	class lMonochrome
	{
	private:
		Color color;

	public:
		class Vertex
		{
		public:
			fVector3D pos;
			fVector3D light;
		public:
			Vertex() = default;
			Vertex(const Monochrome::Vertex& vtx)
				:
				pos(vtx.pos)
			{}
			Vertex(fVector3D vec3, fVector3D light)
				:
				pos(vec3),
				light(light)
			{}
			Vertex(float x, float y, float z, fVector3D light)
				:
				pos(x, y, z),
				light(light)
			{}
			Vertex operator +(const Vertex & vtx) const
			{
				return Vertex(pos + vtx.pos, light + vtx.light);
			}
			Vertex& operator +=(const Vertex & vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex & vtx) const
			{
				return Vertex(pos - vtx.pos, light - vtx.light);
			}
			Vertex& operator -=(const Vertex & vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, light * scale);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, light / scale);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex & vtx, const float alpha)
			{
				return Vertex(pos.InterpolatedTo(vtx.pos, alpha), light.InterpolatedTo(vtx.light, alpha));
			}
		};
	public:
		lMonochrome(Color color)
			:
			color(color)
		{}
		Color operator ()(const Vertex& vtx)
		{
			return fVector3D(color.GetR(), color.GetG(), color.GetB()).GetHadamardProduct(vtx.light);
		}
	};

	// 4. Simple color (lighted)
	class lColoredVertex
	{
	public:
		class Vertex
		{
		public:
			fVector3D pos;
			Color color;
			fVector3D light;
		public:
			Vertex() = default;
			Vertex(const ColoredVertex::Vertex& vtx)
				:
				pos(vtx.pos),
				color(vtx.color)
			{}
			Vertex(dVertex vec3, Color color, fVector3D light)
				:
				pos(vec3.pos),
				color(color),
				light(light)
			{}
			Vertex(float x, float y, float z, Color color, fVector3D light)
				:
				pos(x, y, z),
				color(color),
				light(light)
			{}
			Vertex operator +(const Vertex& vtx) const
			{
				return Vertex(pos + vtx.pos, color, light + vtx.light);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, color, light - vtx.light);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, color, light * scale);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, color, light / scale);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& vtx, const float alpha)
			{
				return Vertex(pos.InterpolatedTo(vtx.pos, alpha), color, light.InterpolatedTo(vtx.light, alpha));
			}
		};
	public:
		Color operator ()(const Vertex& vtx)
		{
			return fVector3D(vtx.color.GetR(), vtx.color.GetG(), vtx.color.GetB()).GetHadamardProduct(vtx.light);
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
// ***Lighted Effects***
typedef PixelShaders::lTexture					ltPIXELSHADER;
typedef PixelShaders::lTexture::Vertex			ltpsVERTEX;
typedef PixelShaders::lVertexBlend				lvbPIXELSHADER;
typedef PixelShaders::lVertexBlend::Vertex		lvbpsVERTEX;
typedef PixelShaders::lMonochrome				lmPIXELSHADER;
typedef PixelShaders::lMonochrome::Vertex		lmpsVERTEX;
typedef PixelShaders::lColoredVertex			lcvPIXELSHADER;
typedef PixelShaders::lColoredVertex::Vertex	lcvpsVERTEX;


typedef Effect3D<tPIXELSHADER, EffectDefaults::VertexShader<tpsVERTEX>, EffectDefaults::GeometryShader<tpsVERTEX>>			tEFFECT_ONLY;
typedef Effect3D<vbPIXELSHADER, EffectDefaults::VertexShader<vbpsVERTEX>, EffectDefaults::GeometryShader<vbpsVERTEX>>		vbEFFECT_ONLY;
typedef Effect3D<mPIXELSHADER, EffectDefaults::VertexShader<mpsVERTEX>, EffectDefaults::GeometryShader<mpsVERTEX>>			mEFFECT_ONLY;
typedef Effect3D<cvPIXELSHADER, EffectDefaults::VertexShader<cvpsVERTEX>, EffectDefaults::GeometryShader<cvpsVERTEX>>		cvEFFECT_ONLY;
// Cannot create stand-alone lighted effects, need a corresponding geometry shader
// ...make sense? ;)

