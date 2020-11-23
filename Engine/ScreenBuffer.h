#pragma once
#include "Graphics.h"
#include "PolyLine.h"
#include "Camera.h"

class ScreenBuffer
{
private:
	Graphics& gfx;
	Camera& camera;

public:
	ScreenBuffer(Graphics& gfx, Camera& camera);
	void DrawLine(fVector2D p0, fVector2D p1, Color c);
	void DrawPolyLine(PolyLine poly, Color c);
};