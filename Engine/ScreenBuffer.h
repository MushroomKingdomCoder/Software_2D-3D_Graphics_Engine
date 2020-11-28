#pragma once
#include "NDCBuffer.h"
#include "Graphics.h"
#include "PolyLine.h"
#include "Matrix.h"
#include "Camera.h"
#include "Prism.h"

class ScreenBuffer
{
private:
	Graphics& gfx;
	Camera& camera;
	NDCBuffer ndc;
private:
	void CullTriangles(TriangleIndexBuffer& tib);
	void CullLines(LineIndexBuffer& lib, TriangleIndexBuffer& tib);

public:
	ScreenBuffer(Graphics& gfx, Camera& camera, NDCBuffer& ndc);
public:
	void DrawLine(fVector2D p0, fVector2D p1, Color c);
	void DrawPolyLine(PolyLine poly, Color c);
public:
	void DrawPrismWireframe(Prism& prism, Color c);
	void DrawPrismRasterized(Prism& prism, Color c);
	void DrawPrismOutline(Prism& prism, Color c);
	void DrawPrismOutlined(Prism& prism, Color c0, Color c1);
};