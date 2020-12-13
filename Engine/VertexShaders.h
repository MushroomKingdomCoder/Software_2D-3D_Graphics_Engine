#pragma once
#include "Effect3D.h"
#include "PixelShaders.h"
#include "Math.h"

namespace VertexShaders
{
	// adds color to monochrome model based on position and uses vertex blending pixel shader
	class Pos2Color
	{
	private:
		fMatrix3D const* pRotation;
		fVector3D const* pTranslation;

	public:
		typedef typename dVertex VertexIn;
		typedef typename vbPIXELSHADER::Vertex VertexOut;
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

	// templated on pixel shader, adds a sine wave effect to the model
	template <class pShader>
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
		typedef typename pShader::Vertex VertexIn;
		typedef typename VertexIn VertexOut;
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
}
typedef EffectDefaults::VertexShader<tPIXELSHADER>	dtVERTEXSHADER;
typedef EffectDefaults::VertexShader<vbPIXELSHADER>	dvbVERTEXSHADER;
typedef EffectDefaults::VertexShader<mPIXELSHADER>	dmVERTEXSHADER;
typedef VertexShaders::Pos2Color					pcVERTEXSHADER;
typedef VertexShaders::SineWave<tPIXELSHADER>		swtVERTEXSHADER;
typedef VertexShaders::SineWave<vbPIXELSHADER>		swvbVERTEXSHADER;
typedef VertexShaders::SineWave<mPIXELSHADER>		swmVERTEXSHADER;


typedef Effect3D<vbPIXELSHADER, pcVERTEXSHADER>		VB_P2C_EFFECT;
typedef Effect3D<tPIXELSHADER, swtVERTEXSHADER>		T_SW_EFFECT;
typedef Effect3D<vbPIXELSHADER, swvbVERTEXSHADER>	VB_SW_EFFECT;
typedef Effect3D<mPIXELSHADER, swmVERTEXSHADER>		M_SW_EFFECT;