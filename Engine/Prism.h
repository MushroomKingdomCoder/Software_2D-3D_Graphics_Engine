#pragma once
#include "IndexBuffer3D.h"
#include "Matrix.h"
#include "Vector.h"
#include "Math.h"
#include <vector>

class Prism
{
private:
	std::vector<fVector3D> Verticies;
	float rot_x = 0.0f;
	float rot_y = 0.0f;
	float rot_z = 0.0f;
	fVector3D Position;

public:
	Prism(std::vector<fVector3D> verts, fVector3D pos = { 0,0,0 });
	LineIndexBuffer GetLIB() const;
	TriangleIndexBuffer GetTIB() const;
public:
	void RotateX(const float radians);
	void RotateY(const float radians);
	void RotateZ(const float radians);
	fMatrix3D RotationMatrix() const;
	void Move(const fVector3D& d_vec3);
	void SetPosition(const fVector3D& pos);
	fVector3D GetPosition() const;
public:
	static Prism MakeCube(float size, fVector3D pos = { 0,0,0 });
};