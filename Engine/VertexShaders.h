#pragma once
#include "Effect3D.h"
#include "PixelShaders.h"
#include "Math.h"

namespace VertexShaders
{
	// 1. adds color to monochrome model based on position and uses vertex blending pixel shader
	class Pos2Color
	{
	private:
		fMatrix3D const* pRotation;
		fVector3D const* pTranslation;

	public:
		typedef typename dVertex VertexIn;
		typedef typename vbpsVERTEX VertexOut;
	public:
		Pos2Color(const fMatrix3D& rotation, const fVector3D& translation)
			:
			pRotation(&rotation),
			pTranslation(&translation)
		{}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.pos = *pRotation * vtx_in.pos;
			vtx_in.pos += *pTranslation;
			return VertexOut(vtx_in.pos, vec3_abs(vtx_in.pos) * 255.0f);
		}
	};

	// 2. templated on vertex type, adds a sine wave effect to the model
	template <typename vertex>
	class SineWave
	{
	private:
		fMatrix3D const* pRotation;
		fVector3D const* pTranslation;
	private:
		float amplitude;
		float time;
		float hz;
		float wavelength;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		SineWave(const fMatrix3D& rot, const fVector3D& trans, float ampl, float hz, float wv)
			:
			pRotation(&rot),
			pTranslation(&trans),
			amplitude(ampl),
			hz(hz),
			wavelength(wv)
		{}
		void UpdateTime(const float seconds)
		{
			time += seconds;
		}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.pos = *pRotation * vtx_in.pos;
			vtx_in.pos += *pTranslation;
			vtx_in.pos.Y += amplitude * sin(hz * time + vtx_in.pos.X * wavelength);
			return VertexOut(vtx_in);
		}
	};

	/*
	
		***Per-Pixel VShaders***
		
	*/

	// 3. adds a sine wave effect to the model (assumes per-pixel lighted vertex type)
	template <typename vertex>
	class SineWave_PPS
	{
	private:
		fMatrix3D const* pRotation;
		fVector3D const* pTranslation;
	private:
		float amplitude;
		float time;
		float hz;
		float wavelength;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		SineWave_PPS(const fMatrix3D& rot, const fVector3D& trans, float ampl, float hz, float wv)
			:
			pRotation(&rot),
			pTranslation(&trans),
			amplitude(ampl),
			hz(hz),
			wavelength(wv)
		{}
		void UpdateTime(const float seconds)
		{
			time += seconds;
		}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.pos = *pRotation * vtx_in.pos;
			vtx_in.normal = *pRotation * vtx_in.normal;
			vtx_in.pos += *pTranslation;
			const float y_stretch = amplitude * sin(hz * time + vtx_in.pos.X * wavelength);
			vtx_in.pos.Y += y_stretch;
			vtx_in.normal.Y += y_stretch;
			vtx_in.World_Pos = vtx_in.pos;
			return VertexOut(vtx_in);
		}
	};
}
typedef EffectDefaults::VertexShader<tpsVERTEX>				dtVERTEXSHADER;
typedef EffectDefaults::VertexShader<vbpsVERTEX>			dvbVERTEXSHADER;
typedef EffectDefaults::VertexShader<mpsVERTEX>				dmVERTEXSHADER;
typedef EffectDefaults::VertexShader<cvpsVERTEX>			dcvVERTEXSHADER;
typedef EffectDefaults::VertexShader_PPS<pptpsVERTEX>		dpptVERTEXSHADER;
typedef VertexShaders::Pos2Color							pcVERTEXSHADER;
typedef VertexShaders::SineWave<tpsVERTEX>					swtVERTEXSHADER;
typedef VertexShaders::SineWave<vbpsVERTEX>					swvbVERTEXSHADER;
typedef VertexShaders::SineWave<mpsVERTEX>					swmVERTEXSHADER;
typedef VertexShaders::SineWave<cvpsVERTEX>					swcvVERTEXSHADER;
typedef VertexShaders::SineWave_PPS<pptpsVERTEX>			swpptVERTEXSHADER;
typedef VertexShaders::SineWave_PPS<ppmpsVERTEX>			swppmVERTEXSHADER;


typedef Effect3D<vbPIXELSHADER, pcVERTEXSHADER, EffectDefaults::GeometryShader<vbpsVERTEX>>			VB_P2C_EFFECT;
typedef Effect3D<tPIXELSHADER, swtVERTEXSHADER, EffectDefaults::GeometryShader<tpsVERTEX>>			T_SW_EFFECT;
typedef Effect3D<vbPIXELSHADER, swvbVERTEXSHADER, EffectDefaults::GeometryShader<vbpsVERTEX>>		VB_SW_EFFECT;
typedef Effect3D<mPIXELSHADER, swmVERTEXSHADER, EffectDefaults::GeometryShader<mpsVERTEX>>			M_SW_EFFECT;
typedef Effect3D<mPIXELSHADER, swcvVERTEXSHADER, EffectDefaults::GeometryShader<cvpsVERTEX>>		CV_SW_EFFECT;
typedef Effect3D<pptPIXELSHADER, swpptVERTEXSHADER, EffectDefaults::GeometryShader<pptpsVERTEX>>	PPT_SW_EFFECT;
typedef Effect3D<ppmPIXELSHADER, swppmVERTEXSHADER, EffectDefaults::GeometryShader<ppmpsVERTEX>>	PPM_SW_EFFECT;

