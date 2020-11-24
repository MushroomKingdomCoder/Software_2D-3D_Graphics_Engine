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
	auto polyline = poly.GetRendered();
	if (fRect(polyline).OverlappedWith(camera.GetViewable())) {
		//Post-Render to Visible Screen from Camera
		fMatrix3D Transformation =
			fMatrix3D::Translation(Graphics::ScreenRect().GetCenter()) *
			fMatrix3D::FlipY() *
			fMatrix3D::Scale(camera.ZoomFactor()) *
			fMatrix3D::Rotation(-camera.TiltAngle()) *
			fMatrix3D::Translation(-camera.Position());
		for (auto& v : polyline) {
			v = Transformation * v;
		}
		gfx.DrawPolyLine(polyline, c);
	}
}
