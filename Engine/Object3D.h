#pragma once
#include "Indexer3D.h"
#include "Matrix.h"
#include "Vector.h"
#include "Math.h"
#include "Triangle.h"
#include <vector>

class Object3D
{
private:
	TriangleIndexer TriangleModel;
private:
	float rot_x = 0.0f;
	float rot_y = 0.0f;
	float rot_z = 0.0f;
	fVector3D Position;
	fMatrix3D Rotation = fMatrix3D::Identity();
private:
	fMatrix3D RotationMatrix();

public:
	Object3D(TriangleIndexer triangle_model, fVector3D pos = { 0,0,0 });
	TriangleIndexer GetTriangleModel() const;
	std::vector<fVector3D> GetVerticies() const;
	std::vector<fTextureVector> GetTextureVerticies() const;
public:
	void RotateX(const float radians);
	void RotateY(const float radians);
	void RotateZ(const float radians);
	const fMatrix3D& GetRotationMatrix();
	void Move(const fVector3D& d_vec3);
	void SetPosition(const fVector3D& pos);
	const fVector3D& GetPosition() const;
public:
	static Object3D MakeTexturedCube(float size, fVector3D pos = { 0,0,0 });
	static Object3D MakeSkinnedCube(float size, fVector3D pos = { 0,0,0 });
	static Object3D MakeWrappingCube(float size, fVector3D pos = { 0,0,0 });
};