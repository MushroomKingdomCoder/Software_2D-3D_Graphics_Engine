#include "ScreenBuffer.h"

ScreenBuffer::ScreenBuffer(Graphics& gfx, Camera& camera)
	:
	gfx(gfx),
	camera(camera)
{}

void ScreenBuffer::DrawLine(fVector2D p0, fVector2D p1, Color c)
{
	DrawPolyLine(PolyLine({ p0,p1 }), c);
}

void ScreenBuffer::DrawPolyLine(PolyLine poly, Color c)
{
	auto& polyline = poly.GetRendered();
	if (fRect(polyline).OverlappedWith(camera.GetViewable())) {
		//Post-Render to Visible Screen from Camera
		const fVector2D offset = fVector2D(Graphics::ScreenRect().GetCenter());
		const float cosR = cos(-camera.TiltAngle());
		const float sinR = sin(-camera.TiltAngle());
		fVector2D cam_pos = camera.Position();
		float zoom = camera.ZoomFactor();
		for (auto& v : polyline) {
			v -= cam_pos;
			v.Rotate(cosR, sinR);
			v *= zoom;
			v.Y *= -1.0f;
			v += offset;
		}
		gfx.DrawPolyLine(polyline, c);
	}
}
