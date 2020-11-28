#include "ScreenBuffer.h"

ScreenBuffer::ScreenBuffer(Graphics& gfx, Camera& camera, NDCBuffer& ndc)
	:
	gfx(gfx),
	camera(camera),
	ndc(ndc)
{}

void ScreenBuffer::CullTriangles(TriangleIndexBuffer& tib)
{
	for (int i = 0; i < tib.Triangles.size(); ++i) {
		tib.Culling[i] = false;
		const auto& v0 = tib.Points[tib.Triangles[i][0]];
		const auto& v1 = tib.Points[tib.Triangles[i][1]];
		const auto& v2 = tib.Points[tib.Triangles[i][2]];
		if ((((v1 - v0) % (v2 - v0)) * v0) >= 0.0f) {
			tib.Culling[i] = true;
		}
	}
}

void ScreenBuffer::CullLines(LineIndexBuffer& lib, TriangleIndexBuffer& tib)
{
	for (int i = 0; i < lib.Lines.size(); ++i) {
		lib.Culling[i] = false;
		const auto& v0 = lib.Points[lib.Lines[i].first];
		const auto& v1 = lib.Points[lib.Lines[i].second];
		const auto l0 = vec3_abs(v1 - v0);
		bool isCulledwithTriangle = true;
		for (int j = 0; j < tib.Triangles.size(); ++j) {
			const auto& t0 = tib.Points[tib.Triangles[j][0]];
			const auto& t1 = tib.Points[tib.Triangles[j][1]];
			const auto& t2 = tib.Points[tib.Triangles[j][2]];
			const auto l1 = vec3_abs(t1 - t0);
			const auto l2 = vec3_abs(t2 - t0);
			if ((l0 == l1 || l0 == l2) && !tib.Culling[j]) {
				isCulledwithTriangle = false;
				break;
			}
		}
		lib.Culling[i] = isCulledwithTriangle;
	}
}

void ScreenBuffer::DrawLine(fVector2D p0, fVector2D p1, Color c)
{
	DrawPolyLine(PolyLine({ p0,p1 }), c);
}

void ScreenBuffer::DrawPolyLine(PolyLine poly, Color c)
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

void ScreenBuffer::DrawPrismWireframe(Prism& prism, Color c)
{
	auto lib = prism.GetLIB();
	for (auto& v : lib.Points) {
		v = prism.RotationMatrix() * v;
		v += prism.GetPosition();
		ndc.Transform(v);
	}
	for (const auto& l : lib.Lines) {
		gfx.DrawLine(lib.Points[l.first], lib.Points[l.second], c);
	}
}

void ScreenBuffer::DrawPrismRasterized(Prism& prism, Color c)
{
	auto tib = prism.GetTIB();
	for (auto& v : tib.Points) {
		v = prism.RotationMatrix() * v;
		v += prism.GetPosition();
	}
	CullTriangles(tib);
	for (auto& v : tib.Points) {
		ndc.Transform(v);
	}
	for (int i = 0; i < tib.Triangles.size(); ++i) {
		if (!tib.Culling[i]) {
			gfx.DrawTriangle(
				tib.Points[tib.Triangles[i][0]],
				tib.Points[tib.Triangles[i][1]],
				tib.Points[tib.Triangles[i][2]], c
			);
		}
	}
}

void ScreenBuffer::DrawPrismOutline(Prism& prism, Color c)
{
	auto lib = prism.GetLIB();
	auto tib = prism.GetTIB();
	for (auto& v : lib.Points) {
		v = prism.RotationMatrix() * v;
		v += prism.GetPosition();
	}
	for (auto& v : tib.Points) {
		v = prism.RotationMatrix() * v;
		v += prism.GetPosition();
	}
	CullTriangles(tib);
	for (auto& v : tib.Points) {
		ndc.Transform(v);
	}
	for (auto& v : lib.Points) {
		ndc.Transform(v);
	}
	CullLines(lib, tib);
	for (int i = 0; i < lib.Lines.size(); ++i) {
		if (!lib.Culling[i]) {
			gfx.DrawLine(
				lib.Points[lib.Lines[i].first],
				lib.Points[lib.Lines[i].second], c
			);
		}
	}
}

void ScreenBuffer::DrawPrismOutlined(Prism& prism, Color c0, Color c1)
{
	auto lib = prism.GetLIB();
	auto tib = prism.GetTIB();
	for (auto& v : lib.Points) {
		v = prism.RotationMatrix() * v;
		v += prism.GetPosition();
	}
	for (auto& v : tib.Points) {
		v = prism.RotationMatrix() * v;
		v += prism.GetPosition();
	}
	CullTriangles(tib);
	for (auto& v : tib.Points) {
		ndc.Transform(v);
	}
	for (auto& v : lib.Points) {
		ndc.Transform(v);
	}
	CullLines(lib, tib);
	for (int i = 0; i < tib.Triangles.size(); ++i) {
		if (!tib.Culling[i]) {
			gfx.DrawTriangle(
				tib.Points[tib.Triangles[i][0]],
				tib.Points[tib.Triangles[i][1]],
				tib.Points[tib.Triangles[i][2]], c0
			);
		}
	}
	for (int i = 0; i < lib.Lines.size(); ++i) {
		if (!lib.Culling[i]) {
			gfx.DrawLine(
				lib.Points[lib.Lines[i].first],
				lib.Points[lib.Lines[i].second], c1
			);
		}
	}
}
