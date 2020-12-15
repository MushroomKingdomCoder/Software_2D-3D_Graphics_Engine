#pragma once
#include "Effect3D.h"
#include "VertexShaders.h"
#include "PixelShaders.h"
#include "Colors.h"
#include "Triangle.h"
#include "Vector.h"

namespace GeometryShaders
{
	// 1. Colors surfaces of a concave shape as pairs of triangles
	class QuadColoring
	{
	private:
		std::vector<Color> colors;

	public:
		typedef typename dVertex VertexIn;
		typedef typename cvpsVERTEX VertexOut;
	public:
		QuadColoring(std::vector<Color> colors)
			:
			colors(colors)
		{}
		Triangle<VertexOut> operator ()(const VertexIn& v0, const VertexIn& v1, const VertexIn& v2, int id)
		{
			return Triangle<VertexOut>(
				VertexOut(v0, colors[int(id / 2)]),
				VertexOut(v1, colors[int(id / 2)]),
				VertexOut(v2, colors[int(id / 2)]));
		}
	};
}
typedef GeometryShaders::QuadColoring		qcGEOMETRYSHADER;


typedef Effect3D<cvPIXELSHADER, EffectDefaults::VertexShader<dVertex>, qcGEOMETRYSHADER> CV_D_QC_EFFECT;