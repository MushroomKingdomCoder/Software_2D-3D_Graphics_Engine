#include "ScreenBuffer.h"

ScreenBuffer::ScreenBuffer(Graphics& gfx, Camera& camera, NDCBuffer& ndc)
	:
	gfx(gfx),
	camera(camera),
	ndc(ndc)
{}

void ScreenBuffer::CullTriangles(TriangleIndexBuffer& tib, const std::vector<fVector3D>& points)
{
	for (int i = 0; i < tib.Triangles.size(); ++i) {
		tib.Culling[i] = false;
		const auto& v0 = points[tib.Triangles[i][0]];
		const auto& v1 = points[tib.Triangles[i][1]];
		const auto& v2 = points[tib.Triangles[i][2]];
		if ((((v1 - v0) % (v2 - v0)).DotProduct(v0) >= 0.0f)) {
			tib.Culling[i] = true;
		}
	}
}

void ScreenBuffer::CullTriangles(TriangleIndexBuffer& tib, const std::vector<fTextureVector>& points)
{
	for (int i = 0; i < tib.Triangles.size(); ++i) {
		tib.Culling[i] = false;
		const auto& v0 = points[tib.Triangles[i][0]].pos;
		const auto& v1 = points[tib.Triangles[i][1]].pos;
		const auto& v2 = points[tib.Triangles[i][2]].pos;
		if ((((v1 - v0) % (v2 - v0)).DotProduct(v0) >= 0.0f)) {
			tib.Culling[i] = true;
		}
	}
}

void ScreenBuffer::CullLines(LineIndexBuffer& lib, TriangleIndexBuffer& tib, const std::vector<fVector3D>& points)
{
	for (int i = 0; i < lib.Lines.size(); ++i) {
		lib.Culling[i] = false;
		const auto& v0 = points[lib.Lines[i].first];
		const auto& v1 = points[lib.Lines[i].second];
		const auto l0 = vec3_abs(v1 - v0);
		bool isCulledwithTriangle = true;
		for (int j = 0; j < tib.Triangles.size(); ++j) {
			const auto& t0 = points[tib.Triangles[j][0]];
			const auto& t1 = points[tib.Triangles[j][1]];
			const auto& t2 = points[tib.Triangles[j][2]];
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

void ScreenBuffer::DrawObject3DWireframe(Object3D& obj, Color c)
{
	auto lib = obj.GetLIB();
	auto points = obj.GetVerticies();
	for (auto& v : points) {
		v = obj.RotationMatrix() * v;
		v += obj.GetPosition();
		ndc.Transform(v);
	}
	for (const auto& l : lib.Lines) {
		gfx.DrawLine(points[l.first], points[l.second], c);
	}
}

void ScreenBuffer::DrawObject3DRasterized(Object3D& obj, Color c)
{
	auto tib = obj.GetTIB();
	auto points = obj.GetVerticies();
	for (auto& v : points) {
		v = obj.RotationMatrix() * v;
		v += obj.GetPosition();
	}
	CullTriangles(tib, points);
	for (auto& v : points) {
		ndc.Transform(v);
	}
	for (int i = 0; i < tib.Triangles.size(); ++i) {
		if (!tib.Culling[i]) {
			gfx.DrawTriangle(
				points[tib.Triangles[i][0]],
				points[tib.Triangles[i][1]],
				points[tib.Triangles[i][2]], c
			);
		}
	}
}

void ScreenBuffer::DrawObject3DOutline(Object3D& obj, Color c)
{
	auto lib = obj.GetLIB();
	auto tib = obj.GetTIB();
	auto points = obj.GetVerticies();
	for (auto& v : points) {
		v = obj.RotationMatrix() * v;
		v += obj.GetPosition();
	}
	CullTriangles(tib, points);
	for (auto& v : points) {
		ndc.Transform(v);
	}
	CullLines(lib, tib, points);
	for (int i = 0; i < lib.Lines.size(); ++i) {
		if (!lib.Culling[i]) {
			gfx.DrawLine(
				points[lib.Lines[i].first],
				points[lib.Lines[i].second], c
			);
		}
	}
}

void ScreenBuffer::DrawObject3DOutlined(Object3D& obj, Color c0, Color c1)
{
	auto lib = obj.GetLIB();
	auto tib = obj.GetTIB();
	auto points = obj.GetVerticies();
	for (auto& v : points) {
		v = obj.RotationMatrix() * v;
		v += obj.GetPosition();
	}
	CullTriangles(tib, points);
	for (auto& v : points) {
		ndc.Transform(v);
	}
	CullLines(lib, tib, points);
	for (int i = 0; i < tib.Triangles.size(); ++i) {
		if (!tib.Culling[i]) {
			gfx.DrawTriangle(
				points[tib.Triangles[i][0]],
				points[tib.Triangles[i][1]],
				points[tib.Triangles[i][2]], c0
			);
		}
	}
	for (int i = 0; i < lib.Lines.size(); ++i) {
		if (!lib.Culling[i]) {
			gfx.DrawLine(
				points[lib.Lines[i].first],
				points[lib.Lines[i].second], c1
			);
		}
	}
}

void ScreenBuffer::DrawTexturedObject3D(Object3D& obj, Sprite& texture)
{
	auto tib = obj.GetTIB();
	auto points = obj.GetTextureVerticies();
	for (auto& v : points) {
		v.pos = obj.RotationMatrix() * v.pos;
		v.pos += obj.GetPosition();
	}
	CullTriangles(tib, points);
	for (auto& v : points) {
		ndc.Transform(v.pos);
	}
	for (int i = 0; i < tib.Triangles.size(); ++i) {
		if (!tib.Culling[i]) {
			gfx.DrawTexturedTriangle(
				points[tib.Triangles[i][0]],
				points[tib.Triangles[i][1]],
				points[tib.Triangles[i][2]], texture
			);
		}
	}
}
