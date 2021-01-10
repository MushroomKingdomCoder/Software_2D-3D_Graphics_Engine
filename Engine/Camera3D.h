#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "Math.h"

class Camera3D
{
private:
	fVector3D position = { 0,0,0 };
	float rot_x = 0.0f;
	float rot_y = 0.0f;
	float rot_z = 0.0f;
	fMatrix3Dplus orientation = fMatrix3Dplus::Identity();

public:
	Camera3D() = default;
	Camera3D(const fVector3D& pos, const fVector3D& orient)
		:
		position(pos),
		rot_x(orient.X),
		rot_y(orient.Y),
		rot_z(orient.Z),
		orientation(
			fMatrix3Dplus::RotationX(rot_x) *
			fMatrix3Dplus::RotationY(rot_y) *
			fMatrix3Dplus::RotationZ(rot_z) *
			fMatrix3Dplus::Identity())
	{}
	void RotateX(const float radians)
	{
		rot_x = angle_wrap(rot_x + radians);
		orientation =
			fMatrix3Dplus::RotationX(rot_x) *
			fMatrix3Dplus::RotationY(rot_y) *
			fMatrix3Dplus::RotationZ(rot_z) *
			fMatrix3Dplus::Identity();
	}
	void RotateY(const float radians)
	{
		rot_y = angle_wrap(rot_y + radians);
		orientation =
			fMatrix3Dplus::RotationX(rot_x) *
			fMatrix3Dplus::RotationY(rot_y) *
			fMatrix3Dplus::RotationZ(rot_z) *
			fMatrix3Dplus::Identity();
	}
	void RotateZ(const float radians)
	{
		rot_z = angle_wrap(rot_z + radians);
		orientation =
			fMatrix3Dplus::RotationX(rot_x) *
			fMatrix3Dplus::RotationY(rot_y) *
			fMatrix3Dplus::RotationZ(rot_z) *
			fMatrix3Dplus::Identity();
	}
	const fMatrix3Dplus& GetOrientationMatrix() const
	{
		return orientation;
	}
	const fMatrix3Dplus GetInverseOrientationMatrix() const
	{
		return !orientation;
	}
	void Move(const fVector3D& translation)
	{
		position += orientation * translation;
	}
	const fVector3D& GetPosition() const
	{
		return position;
	}
	const fVector3D GetInverseTranslation() const
	{
		return -position;
	}
	const fMatrix3Dplus GetCameraTransformationMatrix() const
	{
		return 
			fMatrix3Dplus::Translation(-position) * 
			!orientation * fMatrix3Dplus::Identity();
	}
};







