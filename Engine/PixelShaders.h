#pragma once
#include "Sprite.h"
#include "Colors.h"
#include "Vector.h"
#include "Effect3D.h"
#include "Lighting.h"
#include "Math.h"
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
			fVector4D pos;
			fVector2D tpos;
			fVector3D normal;
		public:
			Vertex() = default;
			Vertex(const fVector4D& pos)
				:
				pos(pos),
				tpos({0,0}),
				normal({0,0,0})
			{}
			Vertex(fVector4D pos, float tx, float ty)
				:
				pos(pos),
				tpos(fVector2D(tx, ty))
			{}
			Vertex(float x, float y, float z, float tx, float ty)
				:
				pos(x, y, z),
				tpos(tx, ty)
			{}
			Vertex(const fVector4D& pos, const fVector2D& tpos, const fVector3D& normal)
				:
				pos(pos),
				tpos(tpos),
				normal(normal)
			{}
			Vertex operator +(const Vertex& tvec) const
			{
				return Vertex(pos + tvec.pos, tpos + tvec.tpos, normal);
			}
			Vertex& operator +=(const Vertex& tvec)
			{
				return *this = *this + tvec;
			}
			Vertex operator -(const Vertex& tvec) const
			{
				return Vertex(pos - tvec.pos, tpos - tvec.tpos, normal);
			}
			Vertex& operator -=(const Vertex & tvec)
			{
				return *this = *this - tvec;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, tpos * scale, normal);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, tpos / scale, normal);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& end, const float alpha) const
			{
				return Vertex(
					fVector3D(pos).InterpolatedTo(end.pos, alpha),
					fVector2D(tpos).InterpolatedTo(end.tpos, alpha), normal
				);
			}
			bool operator ==(const Vertex& vtx) const 
			{
				return (pos == vtx.pos && tpos == vtx.tpos && normal == vtx.normal);
			}
			bool operator !=(const Vertex& vtx) const
			{
				return !(*this == vtx);
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
			fVector4D pos;
			fVector3D color;
			fVector3D normal;
		public:
			Vertex(const fVector4D& pos)
				:
				pos(pos),
				color({0,0,0}),
				normal({0,0,0})
			{}
			Vertex(float x, float y, float z, Color color)
				:
				Vertex({ x,y,z }, fVector3D(float(color.GetR()), float(color.GetG()), float(color.GetB())), { 0,0,0 })
			{}
			Vertex(const fVector4D& pos, const fVector3D& color, const fVector3D& normal)
				:
				pos(pos),
				color(color),
				normal(normal)
			{}
			Vertex operator +(const Vertex& vtx) const
			{
				return Vertex(pos + vtx.pos, color + vtx.color, normal);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, color - vtx.color, normal);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, color * scale, normal);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, color / scale, normal);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& end, const float alpha)
			{
				return Vertex(pos.InterpolatedTo(end.pos, alpha), color.InterpolatedTo(end.color, alpha), normal);
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
			fVector4D pos;
			Color color;
			fVector3D normal;
		public:          
			Vertex(const fVector4D& pos)
				:
				pos(pos),
				color(Colors::White),
				normal({0,0,0})
			{}
			Vertex(const fVector4D& pos, const Color& color, const fVector3D& normal)
				:
				pos(pos),
				color(color),
				normal(normal)
			{}
			Vertex(float x, float y, float z, Color color)
				:
				pos(x, y, z),
				color(color)
			{}
			Vertex operator +(const Vertex& vtx) const
			{
				return Vertex(pos + vtx.pos, color, normal);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, color, normal);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, color, normal);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, color, normal);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& vtx, const float alpha)
			{
				return Vertex(pos.InterpolatedTo(vtx.pos, alpha), color, normal);
			}
		};
	public:
		Color operator ()(const Vertex& vtx)
		{
			return vtx.color;
		}
	};

	/*
	
		***LIGHTED EFFECTS (w/Geometry Shader)***
	
	*/

	// 5. textured (lighted)
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
			fVector4D pos;
			fVector2D tpos;
			fVector3D light;
			fVector3D normal;
		public:
			Vertex() = default;
			Vertex(const Texture::Vertex& vtx)
				:
				pos(vtx.pos),
				tpos(vtx.tpos),
				normal(vtx.normal)
			{}
			Vertex(float x, float y, float z, float tx, float ty, fVector3D light)
				:
				pos(x, y, z),
				tpos(tx, ty),
				light(light)
			{}
			Vertex(fVector4D pos, fVector2D tpos, fVector3D light, fVector3D normal)
				:
				pos(pos),
				tpos(tpos),
				light(light),
				normal(normal)
			{}
			Vertex operator +(const Vertex & tvec) const
			{
				return Vertex(pos + tvec.pos, tpos + tvec.tpos, light + tvec.light, normal);
			}
			Vertex& operator +=(const Vertex & tvec)
			{
				return *this = *this + tvec;
			}
			Vertex operator -(const Vertex & tvec) const
			{
				return Vertex(pos - tvec.pos, tpos - tvec.tpos, light - tvec.light, normal);
			}
			Vertex& operator -=(const Vertex & tvec)
			{
				return *this = *this - tvec;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, tpos * scale, light * scale, normal);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, tpos / scale, light / scale, normal);
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
					fVector3D(light).InterpolatedTo(end.light, alpha), normal
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

	// 6. vertex color blending shader (lighted)
	class lVertexBlend
	{
	public:
		class Vertex
		{
		public:
			fVector4D pos;
			fVector3D color;
			fVector3D light;
			fVector3D normal;
		public:
			Vertex() = default;
			Vertex(const VertexBlend::Vertex& vtx)
				:
				pos(vtx.pos),
				color(vtx.color),
				normal(vtx.normal)
			{}
			Vertex(float x, float y, float z, Color color, fVector3D light)
				:
				Vertex({ x,y,z }, fVector3D(float(color.GetR()), float(color.GetG()), float(color.GetB())), light, { 0,0,0 })
			{}
			Vertex(fVector4D pos, fVector3D color, fVector3D light, fVector3D normal)
				:
				pos(pos),
				color(color),
				light(light),
				normal(normal)
			{}
			Vertex operator +(const Vertex& vtx) const
			{
				return Vertex(pos + vtx.pos, color + vtx.color, light + vtx.light, normal);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, color - vtx.color, light - vtx.light, normal);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, color * scale, light * scale, normal);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, color / scale, light / scale, normal);
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
					light.InterpolatedTo(end.light, alpha), normal);
			}
		};
	public:
		Color operator ()(const Vertex& vtx)
		{
			return Color(vtx.color.GetHadamardProduct(vtx.light));
		}
	};

	// 7. monochrome shading (lighted)
	class lMonochrome
	{
	private:
		Color color;

	public:
		class Vertex
		{
		public:
			fVector4D pos;
			fVector3D light;
			fVector3D normal;
		public:
			Vertex() = default;
			Vertex(const Monochrome::Vertex& vtx)
				:
				pos(vtx.pos),
				normal(vtx.normal)
			{}
			Vertex(const fVector4D& pos, const fVector3D& light, const fVector3D& normal)
				:
				pos(pos),
				light(light),
				normal(normal)
			{}
			Vertex(float x, float y, float z, fVector3D light)
				:
				pos(x, y, z),
				light(light)
			{}
			Vertex operator +(const Vertex & vtx) const
			{
				return Vertex(pos + vtx.pos, light + vtx.light, normal);
			}
			Vertex& operator +=(const Vertex & vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex & vtx) const
			{
				return Vertex(pos - vtx.pos, light - vtx.light, normal);
			}
			Vertex& operator -=(const Vertex & vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, light * scale, normal);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, light / scale, normal);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex & vtx, const float alpha)
			{
				return Vertex(pos.InterpolatedTo(vtx.pos, alpha), light.InterpolatedTo(vtx.light, alpha), normal);
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

	// 8. Simple color (lighted)
	class lColoredVertex
	{
	public:
		class Vertex
		{
		public:
			fVector4D pos;
			Color color;
			fVector3D light;
			fVector3D normal;
		public:
			Vertex() = default;
			Vertex(const ColoredVertex::Vertex& vtx)
				:
				pos(vtx.pos),
				color(vtx.color),
				normal(vtx.normal)
			{}
			Vertex(const fVector4D& pos, const Color& color, const fVector3D& light, const fVector3D& normal)
				:
				pos(pos),
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
				return Vertex(pos + vtx.pos, color, light + vtx.light, normal);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, color, light - vtx.light, normal);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, color, light * scale, normal);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, color, light / scale, normal);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& vtx, const float alpha)
			{
				return Vertex(pos.InterpolatedTo(vtx.pos, alpha), color, light.InterpolatedTo(vtx.light, alpha), normal);
			}
		};
	public:
		Color operator ()(const Vertex& vtx)
		{
			return fVector3D(vtx.color.GetR(), vtx.color.GetG(), vtx.color.GetB()).GetHadamardProduct(vtx.light);
		}
	};

	/*

		***PER-PIXEL LIGHTED EFFECTS***

	*/

	// 9. textured (per-pixel lighting)
	class Texture_PPL
	{
	private:
		Sprite texture;
		const int twidth;
		const int theight;
		bool isNegative;

	public:
		class Vertex
		{
		public:
			fVector4D pos;
			fVector2D tpos;
			fVector3D normal;
			fVector3D World_Pos;
		public:
			Vertex() = default;
			Vertex(const Texture::Vertex& vtx)
				:
				pos(vtx.pos),
				tpos(vtx.tpos),
				normal(vtx.normal),
				World_Pos({ 0,0,0 })
			{}
			Vertex(const fVector4D& pos, const fVector2D& tpos, const fVector3D& n, const fVector3D& wpos)
				:
				pos(pos),
				tpos(tpos),
				normal(n),
				World_Pos(wpos)
			{}
			Vertex operator +(const Vertex& tvec) const
			{
				return Vertex(pos + tvec.pos, tpos + tvec.tpos, normal + tvec.normal, World_Pos + tvec.World_Pos);
			}
			Vertex& operator +=(const Vertex& tvec)
			{
				return *this = *this + tvec;
			}
			Vertex operator -(const Vertex& tvec) const
			{
				return Vertex(pos - tvec.pos, tpos - tvec.tpos, normal - tvec.normal, World_Pos - tvec.World_Pos);
			}
			Vertex& operator -=(const Vertex& tvec)
			{
				return *this = *this - tvec;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, tpos * scale, normal * scale, World_Pos * scale);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, tpos / scale, normal / scale, World_Pos / scale);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& end, const float alpha) const
			{
				return Vertex(
					pos.InterpolatedTo(end.pos, alpha),
					tpos.InterpolatedTo(end.tpos, alpha),
					normal.InterpolatedTo(end.normal, alpha),
					World_Pos.InterpolatedTo(end.World_Pos, alpha)
				);
			}
			bool operator ==(const Vertex& vtx) const
			{
				return (pos == vtx.pos && tpos == vtx.tpos && normal == vtx.normal && World_Pos == vtx.World_Pos);
			}
			bool operator !=(const Vertex& vtx) const
			{
				return !(*this == vtx);
			}
		};
	private:
		const std::vector<Light*>& lights;
	public:
		Texture_PPL() = delete;
		Texture_PPL(std::string file, const std::vector<Light*>& lights, bool isN = false)
			:
			texture(file),
			twidth(texture.getWidth()),
			theight(texture.getHeight()),
			isNegative(isN),
			lights(lights)
		{}
		Color operator ()(const Vertex& vtx)
		{
			Color color = texture.GetPixel(int(vtx.tpos.X * twidth) % twidth, int(vtx.tpos.Y * texture.getHeight()) % theight);
			fVector3D vcolor = { (float)color.GetR(),(float)color.GetG(),(float)color.GetB() };
			fVector3D total_light = { 0,0,0 };
			for (const auto& l : lights) {
				total_light += l->Illuminate(vtx);
			}
			const Color final_color = vcolor.GetHadamardProduct(total_light.Saturated());
			if (!isNegative) {
				return final_color;
			}
			else {
				return -final_color;
			}
		}
	};

	// 10. monochrome shading (per-pixel lighting)
	class Monochrome_PPL
	{
	private:
		fVector3D color;

	public:
		class Vertex
		{
		public:
			fVector4D pos;
			fVector3D normal;
			fVector3D World_Pos;
		public:
			Vertex(const Monochrome::Vertex& vtx)
				:
				pos(vtx.pos),
				normal(vtx.normal),
				World_Pos({0,0,0})
			{}
			Vertex(const fVector4D& pos, const fVector3D& normal, const fVector3D& wpos)
				:
				pos(pos),
				normal(normal),
				World_Pos(wpos)
			{}
			Vertex operator +(const Vertex& vtx) const
			{
				return Vertex(pos + vtx.pos, normal + vtx.normal, World_Pos + vtx.World_Pos);
			}
			Vertex& operator +=(const Vertex& vtx)
			{
				return *this = *this + vtx;
			}
			Vertex operator -(const Vertex& vtx) const
			{
				return Vertex(pos - vtx.pos, normal - vtx.normal, World_Pos - vtx.World_Pos);
			}
			Vertex& operator -=(const Vertex& vtx)
			{
				return *this = *this - vtx;
			}
			Vertex operator *(const float scale) const
			{
				return Vertex(pos * scale, normal * scale, World_Pos * scale);
			}
			Vertex& operator *=(const float scale)
			{
				return *this = *this * scale;
			}
			Vertex operator /(const float scale) const
			{
				return Vertex(pos / scale, normal / scale, World_Pos / scale);
			}
			Vertex& operator /=(const float scale)
			{
				return *this = *this / scale;
			}
			Vertex InterpolatedTo(const Vertex& vtx, const float alpha)
			{
				return Vertex( 
					pos.InterpolatedTo(vtx.pos, alpha), 
					normal.InterpolatedTo(vtx.normal, alpha),
					World_Pos.InterpolatedTo(vtx.World_Pos, alpha)
				);
			}
			bool operator ==(const Vertex& vtx) const
			{
				return (pos == vtx.pos && normal == vtx.normal && World_Pos == vtx.World_Pos);
			}
			bool operator !=(const Vertex& vtx) const
			{
				return !(*this == vtx);
			}
		};
	private:
		const std::vector<Light*>& lights;
	public:
		Monochrome_PPL(Color color, const std::vector<Light*>& lights)
			:
			color(fVector3D(color.GetR(), color.GetG(), color.GetB())),
			lights(lights)
		{}
		Color operator ()(const Vertex& vtx)
		{
			fVector3D total_light = { 0,0,0 };
			for (const auto& l : lights) {
				total_light += l->Illuminate(vtx);
			}
			const Color final_color = color.GetHadamardProduct(total_light.Saturated());
			return final_color;
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
typedef PixelShaders::Texture_PPL				pptPIXELSHADER;
typedef PixelShaders::Texture_PPL::Vertex		pptpsVERTEX;
typedef PixelShaders::Monochrome_PPL			ppmPIXELSHADER;
typedef PixelShaders::Monochrome_PPL::Vertex	ppmpsVERTEX;


typedef Effect3D<tPIXELSHADER, EffectDefaults::VertexShader<tpsVERTEX>, EffectDefaults::GeometryShader<tpsVERTEX>>				tEFFECT_ONLY;
typedef Effect3D<vbPIXELSHADER, EffectDefaults::VertexShader<vbpsVERTEX>, EffectDefaults::GeometryShader<vbpsVERTEX>>			vbEFFECT_ONLY;
typedef Effect3D<mPIXELSHADER, EffectDefaults::VertexShader<mpsVERTEX>, EffectDefaults::GeometryShader<mpsVERTEX>>				mEFFECT_ONLY;
typedef Effect3D<cvPIXELSHADER, EffectDefaults::VertexShader<cvpsVERTEX>, EffectDefaults::GeometryShader<cvpsVERTEX>>			cvEFFECT_ONLY;
typedef Effect3D<pptPIXELSHADER, EffectDefaults::VertexShader_PPS<pptpsVERTEX>, EffectDefaults::GeometryShader<pptpsVERTEX>>	pptEFFECT_ONLY;
typedef Effect3D<ppmPIXELSHADER, EffectDefaults::VertexShader_PPS<ppmpsVERTEX>, EffectDefaults::GeometryShader<ppmpsVERTEX>>	ppmEFFECT_ONLY;
// Cannot create stand-alone lighted effects ( u can w/per-pxl ;) ), need a corresponding geometry shader
// ...make sense? :P
