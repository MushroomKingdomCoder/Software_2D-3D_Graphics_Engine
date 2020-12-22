#pragma once
#include "Colors.h"
#include "Matrix.h"
#include "Vector.h"
#include "Math.h"
#include <functional>

template <typename vertex>
class Light
{
protected:
	fVector3D lighting = { 1,1,1 };
	fVector3D ambience = { 0.1f,0.1f,0.1f };

public:
	std::function<Color(const vertex&, const fVector3D&)> Illumination;
};

template <typename vertex>
class DirectionalLight : public Light<vertex>
{
private:
	fVector3D direction = { 0,0,1 };

public:
	DirectionalLight()
	{
		Illumination = [&](const vertex& pxl, const fVector3D& color) -> Color
		{
			fVector3D light = fVector3D((-lighting * direction.DotProduct(pxl.normal.Normalized())).Saturated() + ambience).Saturated();
			return color.GetHadamardProduct(light);
		};
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
};

template <typename vertex>
class PointLight : public Light<vertex>
{
private:
	const float quadratic_attenuation = 0.75f;
	const float linear_attenuation = 0.2f;
	const float constant_attenuation = 0.05f;
private:
	fVector3D position = { 0,0,1 };

public: 
	PointLight()
	{
		Illumination = [&](const vertex& pxl, const fVector3D& color) -> Color
		{
			const fVector3D delta_dist = position - pxl.World_Pos;
			const float distance = delta_dist.Length();
			const float attenuation = 1.0f / (quadratic_attenuation * sq(distance) + linear_attenuation * distance + constant_attenuation);
			const fVector3D ldirection = delta_dist.Normalized();
			const fVector3D light = ((-lighting * attenuation * ldirection.DotProduct(pxl.normal.Normalized())).Saturated() + ambience).Saturated();
			return color.GetHadamardProduct(light);
		};
	}
	void Move(const fVector3D& translation)
	{
		position += translation;
	}
	const fVector3D& GetPosition() const
	{
		return position;
	}
};

