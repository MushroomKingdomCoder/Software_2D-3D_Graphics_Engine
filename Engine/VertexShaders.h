#pragma once
#include "Effect3D.h"
#include "PixelShaders.h"
#include "Camera3D.h"
#include "Math.h"

namespace VertexShaders
{
	// 1. adds color to monochrome model based on position and uses vertex blending pixel shader
	class Pos2Color
	{
	private:
		fMatrix3Dplus ObjectTransformation;
		fMatrix3Dplus CameraTransformation;
		fMatrix3Dplus Projection;
		fMatrix3Dplus ScreenTransformation;

	public:
		typedef typename dVertex VertexIn;
		typedef typename vbpsVERTEX VertexOut;
	public:
		Pos2Color(const fMatrix3Dplus& o_transform, const fMatrix3Dplus& proj, const fMatrix3Dplus c_transform = fMatrix3Dplus::Identity())
			:
			ObjectTransformation(o_transform),
			CameraTransformation(c_transform),
			Projection(proj),
			ScreenTransformation(Projection * CameraTransformation * ObjectTransformation)
		{}
		VertexOut operator ()(VertexIn vtx_in)
		{
			vtx_in.pos = ScreenTransformation * vtx_in.pos;
			vtx_in.normal = ObjectTransformation * fVector4D(vtx_in.normal, 0.0f);
			return VertexOut(vtx_in.pos, vec3_abs(fVector3D(vtx_in.pos)) * 255.0f, vtx_in.normal);
		}
		void SetProjectionMatrix(const fMatrix3Dplus& projection)
		{
			Projection = projection;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		void SetObjectTransformationMatrix(const fMatrix3Dplus& o_transform)
		{
			ObjectTransformation = o_transform;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		void SetCameraTransformation(const fMatrix3Dplus& c_transform)
		{
			CameraTransformation = c_transform;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		const fMatrix3Dplus& GetProjectionMatrix() const
		{
			return Projection;
		}
	};

	// 2. templated on vertex type, adds a sine wave effect to the model
	template <typename vertex>
	class SineWave
	{
	private:
		fMatrix3Dplus ObjectTransformation;
		fMatrix3Dplus CameraTransformation;
		fMatrix3Dplus Projection;
		fMatrix3Dplus ScreenTransformation;
	private:
		float amplitude;
		float time;
		float hz;
		float wavelength;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		SineWave(const fMatrix3Dplus& o_transform, const fMatrix3Dplus& proj, float ampl, float hz, float wv, const fMatrix3Dplus& c_transform = fMatrix3Dplus::Identity())
			:
			ObjectTransformation(o_transform),
			CameraTransformation(c_transform),
			Projection(proj),
			ScreenTransformation(Projection * CameraTransformation * ObjectTransformation),
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
			const float y_stretch = amplitude * sinf(hz * time + vtx_in.pos.X * wavelength);
			vtx_in.pos.Y += y_stretch;
			vtx_in.normal.Y += y_stretch;
			vtx_in.pos = ScreenTransformation * vtx_in.pos;
			vtx_in.normal = ObjectTransformation * fVector4D(vtx_in.normal, 0.0f);
			return vtx_in;
		}
		void SetProjectionMatrix(const fMatrix3Dplus& projection)
		{
			Projection = projection;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		void SetObjectTransformationMatrix(const fMatrix3Dplus& o_transform)
		{
			ObjectTransformation = o_transform;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		void SetCameraTransformation(const fMatrix3Dplus& c_transform)
		{
			CameraTransformation = c_transform;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		const fMatrix3Dplus& GetProjectionMatrix() const
		{
			return Projection;
		}
	};

	/*
	
		***Per-Pixel VShaders***
		
	*/

	// 3. adds a sine wave effect to the model (assumes per-pixel lighted vertex type -> w/World_Pos member)
	template <typename vertex>
	class SineWave_PPS
	{
	private:
		fMatrix3Dplus ObjectTransformation;
		fMatrix3Dplus CameraTransformation;
		fMatrix3Dplus Projection;
		fMatrix3Dplus ScreenTransformation;
	private:
		float amplitude;
		float time;
		float hz;
		float wavelength;

	public:
		typedef typename vertex VertexIn;
		typedef typename vertex VertexOut;
	public:
		SineWave_PPS(const fMatrix3Dplus& o_transform, const fMatrix3Dplus& proj, float ampl, float hz, float wv, const fMatrix3Dplus& c_transform = fMatrix3Dplus::Identity())
			:
			ObjectTransformation(o_transform),
			CameraTransformation(c_transform),
			Projection(proj),
			ScreenTransformation(Projection * CameraTransformation * ObjectTransformation),
			amplitude(ampl),
			hz(hz),
			wavelength(wv),
			time(0.0f)
		{}
		void UpdateTime(const float seconds)
		{
			time += seconds;
		}
		VertexOut operator ()(VertexIn vtx_in)
		{
			const float y_stretch = amplitude * sin(hz * time + vtx_in.pos.X * wavelength);
			vtx_in.pos.Y += y_stretch;
			vtx_in.normal.Y += y_stretch;
			vtx_in.World_Pos = ObjectTransformation * vtx_in.pos;
			vtx_in.pos = ScreenTransformation * vtx_in.pos;
			vtx_in.normal = ObjectTransformation * fVector4D(vtx_in.normal, 0.0f);
			return vtx_in;
		}
		void SetProjectionMatrix(const fMatrix3Dplus& projection)
		{
			Projection = projection;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		void SetObjectTransformationMatrix(const fMatrix3Dplus& o_transform)
		{
			ObjectTransformation = o_transform;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		void SetCameraTransformation(const fMatrix3Dplus& c_transform)
		{
			CameraTransformation = c_transform;
			ScreenTransformation = Projection * CameraTransformation * ObjectTransformation;
		}
		const fMatrix3Dplus& GetProjectionMatrix() const
		{
			return Projection;
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

