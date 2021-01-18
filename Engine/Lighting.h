#pragma once
#include "Colors.h"
#include "Matrix.h"
#include "Vector.h"
#include "Math.h"

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
	float specular = 0.5f;
	unsigned int specular_power = 100;

public:
	virtual fVector3D Illuminate(PerPixelLightingVertex pxl) const = 0;
};

class DirectionalLight : public Light
{
private:
	fVector3D direction = { 0,0,1 };

public:
	DirectionalLight() = default;
	DirectionalLight(fVector3D l, fVector3D a, float s, unsigned int sp, fVector3D d)
		:
		direction(d) 
	{
		lighting = l;
		ambience = a;
		specular = s;
		specular_power = sp;
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
	fVector3D Illuminate(PerPixelLightingVertex pxl) const override
	{
		const fVector3D diffuse = lighting * std::max(-direction.DotProduct(pxl.normal), 0.0f);
		const fVector3D reflected = pxl.normal * (-direction.DotProduct(pxl.normal)) + direction;
		const fVector3D specular_light = lighting * specular * (float)pow(std::max(pxl.World_Pos.Normalized().DotProduct(direction), 0.0f), specular_power);
		return (diffuse + ambience).Saturated() + specular_light;
	}
};

class PointLight : public Light
{
private:
	const float quadratic_attenuation = 0.125f;
	const float linear_attenuation = 0.750f;
	const float constant_attenuation = 0.125f;
private:
	fVector3D position = { 0,0,1 };

public: 
	PointLight() = default;
	PointLight(fVector3D l, fVector3D a, float s, unsigned int sp, float qa, float la, float ca, fVector3D pos)
		:
		quadratic_attenuation(qa),
		linear_attenuation(la),
		constant_attenuation(ca),
		position(pos)
	{
		lighting = l;
		ambience = a;
		specular = s;
		specular_power = sp;
	}
	void Move(const fVector3D& translation)
	{
		position += translation;
	}
	void SetPosition(const fVector3D& pos)
	{
		position = pos;
	}
	const fVector3D& GetPosition() const
	{
		return position;
	}
	fVector3D Illuminate(PerPixelLightingVertex pxl) const override
	{
		const fVector3D delta_dist = position - pxl.World_Pos;
		const float distance = delta_dist.Length();
		const float attenuation = 1.0f / (quadratic_attenuation * sq(distance) + linear_attenuation * distance + constant_attenuation);
		const fVector3D ldirection = delta_dist / distance;
		const fVector3D diffuse = lighting * attenuation * std::max(ldirection.DotProduct(pxl.normal), 0.0f);
		const fVector3D reflected = pxl.normal * (2 * delta_dist.DotProduct(pxl.normal)) - delta_dist;
		const fVector3D specular_light = lighting * specular * (float)pow(std::max(-pxl.World_Pos.Normalized().DotProduct(reflected / distance), 0.0f), specular_power);
		return (diffuse + ambience).Saturated() + specular_light;
	}
};

