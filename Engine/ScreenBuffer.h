#pragma once
#include "NDCBuffer.h"
#include "Graphics.h"
#include "PolyLine.h"
#include "Matrix.h"
#include "Vector.h"
#include "Camera.h"
#include "Sprite.h"
#include "Object3D.h"

class ScreenBuffer
{
private:
	Graphics& gfx;
	Camera& camera;
	NDCBuffer& ndc;
private:
	void CullTriangles(TriangleIndexBuffer& tib, const std::vector<fVector3D>& points);
	void CullTriangles(TriangleIndexBuffer& tib, const std::vector<fTextureVector>& points);
	void CullLines(LineIndexBuffer& lib, TriangleIndexBuffer& tib, const std::vector<fVector3D>& points);

public:
	ScreenBuffer(Graphics& gfx, Camera& camera, NDCBuffer& ndc);
public:
	void DrawLine(fVector2D p0, fVector2D p1, Color c);
	void DrawPolyLine(PolyLine poly, Color c);
public:
	void DrawObject3DWireframe(Object3D& obj, Color c);
	void DrawObject3DRasterized(Object3D& obj, Color c);
	void DrawObject3DOutline(Object3D& obj, Color c);
	void DrawObject3DOutlined(Object3D& obj, Color c0, Color c1);
public:
	void DrawTexturedObject3D(Object3D& obj, Sprite& texture);
};