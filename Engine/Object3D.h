#pragma once
#include "IndexBuffer3D.h"
#include "Matrix.h"
#include "Vector.h"
#include "Math.h"
#include <vector>

class Object3D
{
private:
	std::vector<fVector3D> Verticies;
	std::vector<fTextureVector> tVerticies;
	LineIndexBuffer LIB;
	TriangleIndexBuffer TIB;
private:
	float rot_x = 0.0f;
	float rot_y = 0.0f;
	float rot_z = 0.0f;
	fVector3D Position;

public:
	Object3D(std::vector<fVector3D> verts, LineIndexBuffer lib, TriangleIndexBuffer tib, fVector3D pos = { 0,0,0 });
	Object3D(std::vector<fTextureVector> tverts, TriangleIndexBuffer tib, fVector3D pos = { 0,0,0 });
	LineIndexBuffer GetLIB() const;
	TriangleIndexBuffer GetTIB() const;
	std::vector<fVector3D> GetVerticies() const;
	std::vector<fTextureVector> GetTextureVerticies() const;
public:
	void RotateX(const float radians);
	void RotateY(const float radians);
	void RotateZ(const float radians);
	fMatrix3D RotationMatrix() const;
	void Move(const fVector3D& d_vec3);
	void SetPosition(const fVector3D& pos);
	fVector3D GetPosition() const;
public:
	static Object3D MakeCube(float size, fVector3D pos = { 0,0,0 });
	static Object3D MakeTexturedCube(float size, fVector3D pos = { 0,0,0 });
};