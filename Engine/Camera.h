#pragma once
#include "Graphics.h"
#include "Vector2D.h"
#include "Rect.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Camera
{
private:
	fVector2D Pos = { 0.0f,0.0f };
	float Zoom = 1.0f;
	float Angle = 0.0f;
	fRect Viewport = PolyLine::MakeCircle((Graphics::ScreenRectFromCenter().GetTopLeft().Length())).GetRect();

public:
	Camera() = default;
	Camera(fVector2D pos, float zoom)
		:
		Pos(pos),
		Zoom(zoom)
	{}
	void Pan(fVector2D delta_pan)
	{
		Pos += delta_pan;
	}
	void Cut_To(fVector2D pos)
	{
		Pos = pos;
	}
	void Zoom_By(float zoom_factor)
	{
		Zoom *= zoom_factor;
	}
	void Tilt(const float angle)
	{
		Angle += angle;
		while (Angle > M_PI) {
			Angle -= 2.0f * M_PI;
		}
	}
	void SetTilt(const float angle)
	{
		Angle = angle;
		while (Angle > M_PI) {
			Angle -= 2.0f * M_PI;
		}
	}
	fVector2D Position()
	{
		return Pos;
	}
	float ZoomFactor()
	{
		return Zoom;
	}
	float TiltAngle() const
	{
		return Angle;
	}
	const fRect GetViewable()
	{
		auto viewport = Viewport;
		viewport.Scale(1.0f / Zoom);
		viewport.Move(Pos);
		return viewport;
	}
};
