#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

// ***Forward Declare Vector3D***
template <typename type>
class Vector3D;
// ------------------------------

// ***Forward Declare Vector4D***
template <typename type>
class Vector4D;
// ------------------------------


/*

	Vector2D

*/


template <typename type>
class Vector2D
{
public:
	type X;
	type Y;
public:
	Vector2D() = default;
	Vector2D(const type x, const type y)
	{
		X = x; Y = y;
	}
	template <typename sType>
	Vector2D(const Vector2D<sType>& S)
		:
		X((type)S.X),
		Y((type)S.Y)
	{}
	template <typename vType>
	Vector2D(const Vector3D<vType>& vec3)
		:
		X((type)vec3.X),
		Y((type)vec3.Y)
	{}
	template <typename vType>
	Vector2D(const Vector4D<vType>& vec4)
		:
		X((type)vec4.X),
		Y((type)vec4.Y)
	{}
	Vector2D operator +(const Vector2D& addend) const
	{
		return { X + addend.X, Y + addend.Y };
	}
	Vector2D& operator +=(const Vector2D& addend)
	{
		return *this = *this + addend;
	}
	Vector2D operator *(const type scale) const
	{
		return Vector2D( X * scale, Y * scale );
	}
	Vector2D& operator *=(const type scale)
	{
		return *this = *this * scale;
	}
	Vector2D operator -(const Vector2D& dif) const
	{
		return { X - dif.X, Y - dif.Y };
	}
	Vector2D& operator -=(const Vector2D& dif)
	{
		return *this = *this - dif;
	}
	Vector2D operator /(const type scale) const
	{
		return Vector2D(X / scale, Y / scale);
	}
	Vector2D& operator /=(const type scale)
	{
		return *this = *this / scale;
	}
	Vector2D operator -() const
	{
		return Vector2D(-(this->X), -(this->Y));
	}
	type DotProduct(const Vector2D& vec) const
	{
		return type(X * vec.X + Y * vec.Y);
	}
	Vector2D& Rotate(const float radians)
	{
		return *this = this->Rotated(radians);
	}
	Vector2D Rotated(const float radians)
	{
		const float cosR = cos(radians);
		const float sinR = sin(radians);
		return Vector2D(X * cosR - Y * sinR, X * sinR + Y * cosR);
	}
	Vector2D& Rotate(const float cosR, const float sinR)
	{
		return *this = this->Rotated(cosR, sinR);
	}
	Vector2D Rotated(const float cosR, const float sinR)
	{
		return Vector2D(X * cosR - Y * sinR, X * sinR + Y * cosR);
	}
	Vector2D InterpolatedTo(const Vector2D& end, const float alpha) const
	{
		return (*this + (end - *this) * alpha);
	}
	Vector2D InterpolatedTo(const Vector2D& point, const Vector2D& end) const
	{
		const float alpha = end.Y - Y != 0 ? (point.Y - Y) / (end.Y - Y) : 0;
		return (*this + (end - *this) * alpha);
	}
	Vector2D Normalized() const
	{
		const type len = Length();
		if (len != (type)0.0f) {
			return *this * ((type)1.0f / len);
		}
		return { (type)0,(type)0 };
	}
	Vector2D& Normalize()
	{
		return *this = Normalized();
	}
	float Length() const
	{
		return float(sqrt(pow(X, 2) + pow(Y, 2)));
	}
	float LengthSq() const
	{
		return float(pow(X, 2) + pow(Y, 2));
	}
	Vector2D& ReboundOff(const Vector2D& vec)
	{
		return *this = ((vec * (*this * vec)) * 2.0f) - *this;
	}
	type DistanceBetweenLine(Vector2D p0, Vector2D p1)
	{
		const type a = p0.Y - p1.Y;
		const type b = -(p0.X - p1.X);
		const type c = p0.X * p1.Y - p1.X * p0.Y;
		const type distance = abs(a * X + b * Y + c) / sqrt(pow(a, 2) + pow(b, 2));
		return distance;
	}
	Vector2D GetLineNormal(std::pair<Vector2D, Vector2D> Line)
	{
		Vector2D v = Vector2D(Line.second - Line.first).Normalize();
		v.RotateN90();
		return v;
	}
	std::vector<Vector2D> GetPointsBetween(Vector2D p1)
	{
		std::vector<Vector2D> LinePoints;
		Vector2D p0 = *this;

		const float rise = p1.Y - p0.Y;
		const float run = p1.X - p0.X;
		if (std::abs(rise) < std::abs(run))
		{
			if (p0.X > p1.X) {
				std::swap(p0, p1);
			}
			const float m = rise / run;
			const float b = p0.Y - m * p0.X;
			for (int x = (int)p0.X; x <= (int)p1.X; ++x) {
				const float y = m * x + b;
				LinePoints.emplace_back(Vector2D((float)x, y));
			}
		}
		else
		{
			if (p0.Y > p1.Y) {
				std::swap(p0, p1);
			}
			const float w = run / rise;
			const float p = p0.X - w * p0.Y;
			for (int y = (int)p0.Y; y <= (int)p1.Y; ++y) {
				const float x = w * y + p;
				LinePoints.emplace_back(Vector2D(x, (float)y));
			}
		}
		return LinePoints;
	}
	void Rotate90()
	{
		X = -Y; Y = X;
	}
	void Rotate180()
	{
		X = -X; Y = -Y;
	}
	void RotateN90()
	{
		X = Y; Y = -X;
	}
	bool operator ==(const Vector2D& vec2) const
	{
		return (X == vec2.X && Y == vec2.Y);
	}
	bool operator !=(const Vector2D& vec2) const
	{
		return !(*this == vec2);
	}
public:
	const type& operator [](unsigned int i) const
	{
		i = i % 2;
		if (i == 0) {
			return X;
		}
		else {
			return Y;
		}
	}
	type& operator [](unsigned int i)
	{
		i = i % 2;
		if (i == 0) {
			return X;
		}
		else {
			return Y;
		}
	}
};
using fVector2D = Vector2D<float>;
using dvector2D = Vector2D<double>;
using iVector2D = Vector2D<int>;


/*

	Vector3D

*/


template <typename type>
class Vector3D
{
public:
	type X;
	type Y;
	type Z = type(1);

public:
	Vector3D() = default;
	Vector3D(const type x, const type y, const type z = 1)
	{
		X = x; Y = y; Z = z;
	}
	template <typename sType>
	Vector3D(const Vector3D<sType>& S)
		:
		X((type)S.X),
		Y((type)S.Y),
		Z((type)S.Z)
	{}
	template <typename vType>
	Vector3D(const Vector2D<vType>& vec2)
		:
		X((type)vec2.X),
		Y((type)vec2.Y),
		Z(type(1))
	{}
	template <typename vType>
	Vector3D(const Vector4D<vType>& vec4)
		:
		X((type)vec4.X),
		Y((type)vec4.Y),
		Z((type)vec4.Z)
	{}
	Vector3D operator +(const Vector3D& addend) const
	{
		return { X + addend.X, Y + addend.Y, Z + addend.Z };
	}
	Vector3D& operator +=(const Vector3D& addend)
	{
		return *this = *this + addend;
	}
	Vector3D operator *(const type scale) const
	{
		return Vector3D(X * scale, Y * scale, Z * scale);
	}
	Vector3D& operator *=(const type scale)
	{
		return *this = *this * scale;
	}
	Vector3D operator -(const Vector3D& dif) const
	{
		return { X - dif.X, Y - dif.Y, Z - dif.Z };
	}
	Vector3D& operator -=(const Vector3D& dif)
	{
		return *this = *this - dif;
	}
	Vector3D operator /(const type scale) const
	{
		return Vector3D(X / scale, Y / scale, Z / scale);
	} 
	Vector3D& operator /=(const type scale)
	{
		return *this = *this / scale;
	}
	Vector3D operator -() const
	{
		return Vector3D(-X, -Y, -Z);
	}
	type DotProduct(const Vector3D& vec) const
	{
		return type(X * vec.X + Y * vec.Y + Z * vec.Z);
	}
	Vector3D operator %(const Vector3D& vec3) const
	{
		return Vector3D(
			((Y * vec3.Z) - (Z * vec3.Y)),
			((Z * vec3.X) - (X * vec3.Z)),
			((X * vec3.Y) - (Y * vec3.X))
		);
	}
	Vector3D InterpolatedTo(const Vector3D& end, const float alpha) const
	{
		return (*this + (end - *this) * alpha);
	}
	Vector3D InterpolatedTo(const Vector3D& point, const Vector3D& end) const
	{
		const float alpha = end.Y - Y != 0 ? (point.Y - Y) / (end.Y - Y) : 0;
		return (*this + (end - *this) * alpha);
	}
	type Length() const
	{
		return (type)sqrt(LengthSq());
	}
	type LengthSq() const
	{
		return type(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
	}
	Vector3D Normalized() const
	{
		const type len = Length();
		if (len != (type)0) {
			return *this / len;
		}
		return Vector3D( (type)0,(type)0,(type)0 );
	}
	Vector3D& Normalize()
	{
		return *this = Normalized();
	}
	Vector3D GetHadamardProduct(const Vector3D& vec3) const
	{
		return Vector3D(
			X * vec3.X, Y * vec3.Y, Z * vec3.Z
		);
	}
	Vector3D& Hadamard(const Vector3D& vec3)
	{
		return *this = GetHadamardProduct(vec3);
	}
	Vector3D Saturated() const
	{
		return Vector3D(
			std::min(std::max(X, (type)0), (type)1),
			std::min(std::max(Y, (type)0), (type)1),
			std::min(std::max(Z, (type)0), (type)1)
		);
	}
	Vector3D& Saturate()
	{
		return *this = Saturated();
	}
	bool operator ==(const Vector3D& vec3) const
	{
		return (X == vec3.X && Y == vec3.Y && Z == vec3.Z);
	}
	bool operator !=(const Vector3D& vec3) const
	{
		return !(*this == vec3);
	}
public:
	const type& operator [](unsigned int i) const
	{
		i = i % 3;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else {
			return Z;
		}
	}
	type& operator [](unsigned int i)
	{
		i = i % 3;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else {
			return Z;
		}
	}
};
using fVector3D = Vector3D<float>;
using dVector3D = Vector3D<double>;
using iVector3D = Vector3D<int>;


/*

	Vector4D

*/


template <typename type>
class Vector4D
{
public:
	type X;
	type Y;
	type Z;
	type W = (type)1;
public:
	Vector4D(type x, type y, type z, type w)
		:
		X(x),
		Y(y),
		Z(z),
		W(w)
	{}
	template <typename vType>
	Vector4D(const Vector4D<vType> vec4)
		:
		X((type)vec4.X),
		Y((type)vec4.Y),
		Z((type)vec4.Z),
		W((type)vec4.W)
	{}
	template <typename vType>
	Vector4D(const Vector2D<vType>& vec2)
		:
		X((type)vec2.X),
		Y((type)vec2.Y),
		Z((type)0.0),
		W((type)1.0)
	{}
	template <typename vType>
	Vector4D(const Vector3D<vType>& vec3)
		:
		X((type)vec3.X),
		Y((type)vec3.Y),
		Z((type)vec3.Z),
		W((type)1.0)
	{}
	template <typename vType>
	Vector4D(const Vector3D<vType>& vec3, type w)
		:
		X((type)vec3.X),
		Y((type)vec3.Y),
		Z((type)vec3.Z),
		W(w)
	{}
	Vector4D operator +(const Vector4D& addend) const
	{
		return Vector4D( X + addend.X, Y + addend.Y, Z + addend.Z, W + addend.W );
	}
	Vector4D& operator +=(const Vector4D& addend)
	{
		return *this = *this + addend;
	}
	Vector4D operator *(const type scale) const
	{
		return Vector4D(X * scale, Y * scale, Z * scale, W * scale);
	}
	Vector4D& operator *=(const type scale)
	{
		return *this = *this * scale;
	}
	Vector4D operator -(const Vector4D& dif) const
	{
		return Vector4D( X - dif.X, Y - dif.Y, Z - dif.Z, W - dif.W);
	}
	Vector4D& operator -=(const Vector4D& dif)
	{
		return *this = *this - dif;
	}
	Vector4D operator /(const type scale) const
	{
		return Vector4D(X / scale, Y / scale, Z / scale, W / scale);
	}
	Vector4D& operator /=(const type scale)
	{
		return *this = *this / scale;
	}
	Vector4D operator -() const
	{
		return Vector4D(-X, -Y, -Z, -W);
	}
	type DotProduct(const Vector4D& vec) const
	{
		return type(X * vec.X + Y * vec.Y + Z * vec.Z + W * vec.W);
	}
	Vector4D InterpolatedTo(const Vector4D& end, const float alpha) const
	{
		return Vector4D(*this + (end - *this) * alpha);
	}
	Vector4D InterpolatedTo(const Vector4D& point, const Vector4D& end) const
	{
		const float alpha = end.Y - Y != 0 ? (point.Y - Y) / (end.Y - Y) : 0;
		return Vector4D(*this + (end - *this) * alpha);
	}
	Vector4D GetHadamardProduct(const Vector4D& vec4) const
	{
		return Vector4D(
			X * vec4.X, Y * vec4.Y, Z * vec4.Z, W * vec4.W;
		);
	}
	Vector4D& Hadamard(const Vector4D& vec4)
	{
		return *this = GetHadamardProduct(vec4);
	}
	Vector4D Saturated() const
	{
		return Vector4D(
			std::min(std::max(X, (type)0), (type)1),
			std::min(std::max(Y, (type)0), (type)1),
			std::min(std::max(Z, (type)0), (type)1),
			std::min(std::max(W, (type)0), (type)1)
		);
	}
	Vector4D& Saturate()
	{
		return *this = Saturated();
	}
	bool operator ==(const Vector4D& vec4) const
	{
		return (X == vec4.X && Y == vec4.Y && Z == vec4.Z && W == vec4.W);
	}
	bool operator !=(const Vector4D& vec4) const
	{
		return !(*this == vec4);
	}
public:
	const type& operator [](unsigned int i) const
	{
		i = i % 4;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else if (i == 2) {
			return Z;
		}
		else {
			return W;
		}
	}
	type& operator [](unsigned int i)
	{
		i = i % 4;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else if (i == 2) {
			return Z;
		}
		else {
			return W;
		}
	}
};
using fVector4D = Vector4D<float>;
using dVector4D = Vector4D<double>;
using iVector4D = Vector4D<int>;




