#pragma once
#include "Graphics.h"
#include "PolyLine.h"
#include "Matrix.h"
#include "Vector.h"
#include "Camera.h"
#include "Sprite.h"

class ScreenBuffer
{
private:
	Graphics& gfx;
	Camera& camera;

public:
	ScreenBuffer(Graphics& gfx, Camera& camera)
		:
		gfx(gfx),
		camera(camera)
	{}
	void DrawLine(fVector2D p0, fVector2D p1, Color c)
	{
		DrawPolyLine(PolyLine({ p0,p1 }), c);
	}
	void DrawPolyLine(PolyLine poly, Color c)
	{
		auto polyline = poly.GetRendered();
		if (fRect(polyline).OverlappedWith(camera.GetViewable())) {
			//Post-Render to Visible Screen from Camera
			fMatrix2Dplus Transformation =
				fMatrix2Dplus::Translation(Graphics::ScreenRect().GetCenter()) *
				fMatrix2Dplus::FlipY() *
				fMatrix2Dplus::Scale(camera.ZoomFactor()) *
				fMatrix2Dplus::Rotation(-camera.TiltAngle()) *
				fMatrix2Dplus::Translation(-camera.Position());
			for (auto& v : polyline) {
				v = Transformation * v;
			}
			gfx.DrawPolyLine(polyline, c);
		}
	}
};