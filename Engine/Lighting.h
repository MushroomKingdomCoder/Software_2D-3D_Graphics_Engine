#pragma once
#include "Colors.h"
#include "Matrix.h"
#include "Vector.h"
#include "Math.h"
#include <functional>

class PerPixelLightingVertex
{
public:
	fVector3D normal;
	fVector3D World_Pos;
public:
	// construct from any per-pixel lighted vertex class
	template <typename vertex>
	PerPixelLightingVertex(vertex vtx)
		:
		normal(vtx.normal),
		World_Pos(vtx.World_Pos)
	{}
};


class Light
{
protected:
	fVector3D lighting = { 1,1,1 };
	fVector3D ambience = { 0.1f,0.1f,0.1f };

public:
	virtual Color Illuminate(PerPixelLightingVertex pxl, const fVector3D& color) const = 0;
};

class DirectionalLight : public Light
{
private:
	fVector3D direction = { 0,0,1 };

public:
	DirectionalLight() = default;
	DirectionalLight(fVector3D l, fVector3D a, fVector3D d)
		:
		direction(d) 
	{
		lighting = l;
		ambience = a;
	}
	void RotateLightX(const float radians)
	{
		direction = (fMatrix3D::RotationX(radians) * direction).Normalized();
	}
	void RotateLightY(const float radians)
	{
		direction = (fMatrix3D::RotationY(radians) * direction).Normalized();
	}
	void RotateLightZ(const float radians)
	{
		direction = (fMatrix3D::RotationZ(radians) * direction).Normalized();
	}
	Color Illuminate(PerPixelLightingVertex pxl, const fVector3D& color) const override
	{
		fVector3D light = fVector3D((-lighting * direction.DotProduct(pxl.normal.Normalized())).Saturated() + ambience).Saturated();
		return color.GetHadamardProduct(light);
	}
};

class PointLight : public Light
{
private:
	const float quadratic_attenuation = 0.75f;
	const float linear_attenuation = 0.2f;
	const float constant_attenuation = 0.05f;
private:
	fVector3D position = { 0,0,1 };

public: 
	PointLight() = default;
	PointLight(fVector3D l, fVector3D a, float qa, float la, float ca, fVector3D pos)
		:
		quadratic_attenuation(qa),
		linear_attenuation(la),
		constant_attenuation(ca),
		position(pos)
	{
		lighting = l;
		ambience = a;
	}
	void Move(const fVector3D& translation)
	{
		position += translation;
	}
	const fVector3D& GetPosition() const
	{
		return position;
	}
	Color Illuminate(PerPixelLightingVertex pxl, const fVector3D& color) const override
	{
		const fVector3D delta_dist = position - pxl.World_Pos;
		const float distance = delta_dist.Length();
		const float attenuation = 1.0f / (quadratic_attenuation * sq(distance) + linear_attenuation * distance + constant_attenuation);
		const fVector3D ldirection = delta_dist.Normalized();
		const fVector3D light = ((-lighting * attenuation * ldirection.DotProduct(pxl.normal.Normalized())).Saturated() + ambience).Saturated();
		return color.GetHadamardProduct(light);
	}
};

