#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

// ***Forward Declare Vector3D***
template <typename type>
class Vector3D;
// ------------------------------

// ***Forward Declare TextureVector***
template <typename type>
class TextureVector;
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
	Vector2D(Vector2D<sType> S)
		:
		X((type)S.X),
		Y((type)S.Y)
	{}
	template <typename vType>
	Vector2D(Vector3D<vType> vec3)
		:
		X((type)vec3.X),
		Y((type)vec3.Y)
	{}
	template <typename tType>
	Vector2D(TextureVector<tType> tvec)
		:
		Vector2D(tvec.pos)
	{}
	Vector2D operator+(const Vector2D& addend) const
	{
		return { X + addend.X, Y + addend.Y };
	}
	Vector2D& operator+=(const Vector2D& addend)
	{
		return *this = *this + addend;
	}
	Vector2D operator*(const type scale) const
	{
		return { X * scale, Y * scale };
	}
	Vector2D& operator*=(const type scale)
	{
		return *this = *this * scale;
	}
	Vector2D operator *(const Vector2D& vec_scale) const
	{
		return Vector2D(X * vec_scale.X, Y * vec_scale.Y);
	}
	Vector2D& operator *=(const Vector2D vec_scale)
	{
		return *this = *this * vec_scale;
	}
	Vector2D operator-(const Vector2D& dif) const
	{
		return { X - dif.X, Y - dif.Y };
	}
	Vector2D& operator-=(const Vector2D& dif)
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
	Vector2D operator /(const Vector2D& vec_scale) const
	{
		return Vector2D(X / vec_scale.X, Y / vec_scale.Y);
	}
	Vector2D& operator /=(const Vector2D& vec_scale)
	{
		return *this = *this / vec_scale;
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
	Vector2D InterpolatedTo(const Vector2D& point, const Vector2D& end, const float alpha) const
	{
		return (*this + (end - *this) * alpha);
	}
	Vector2D InterpolatedTo(const Vector2D& point, const Vector2D& end) const
	{
		const float alpha = (point.Y - Y) / (end.Y - Y);
		return (*this + (end - *this) * alpha);
	}
	Vector2D Normalized()
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
	float Length()
	{
		return float(sqrt(pow(X, 2) + pow(Y, 2)));
	}
	float LengthSq()
	{
		return float(pow(X, 2) + pow(Y, 2));
	}
	void ReboundOff(const Vector2D& vec)
	{
		*this = ((vec * (*this * vec)) * 2.0f) - *this;
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
	const type& operator [](int i) const
	{
		i = i % 2;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		return X;
	}
	type& operator [](int i)
	{
		i = i % 2;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		return X;
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
	Vector3D(Vector3D<sType> S)
		:
		X((type)S.X),
		Y((type)S.Y),
		Z((type)S.Z)
	{}
	template <typename vType>
	Vector3D(Vector2D<vType> vec2)
		:
		X((type)vec2.X),
		Y((type)vec2.Y),
		Z(type(1))
	{}
	template <typename tType>
	Vector3D(TextureVector<tType> tvec)
		:
		Vector3D(tvec.pos)
	{}
	Vector3D operator+(const Vector3D& addend) const
	{
		return { X + addend.X, Y + addend.Y, Z + addend.Z };
	}
	Vector3D& operator+=(const Vector3D& addend)
	{
		return *this = *this + addend;
	}
	Vector3D operator*(const type scale) const
	{
		return { X * scale, Y * scale, Z * scale };
	}
	Vector3D& operator*=(const type scale)
	{
		return *this = *this * scale;
	}
	Vector3D operator *(const Vector3D& vec3_scale) const
	{
		return Vector3D(X * vec3_scale.X, Y * vec3_scale.Y, Z * vec3_scale.Z);
	}
	Vector3D& operator *=(const Vector3D& vec3_scale)
	{
		return *this = *this * vec3_scale;
	}
	Vector3D operator-(const Vector3D& dif) const
	{
		return { X - dif.X, Y - dif.Y, Z - dif.Z };
	}
	Vector3D& operator-=(const Vector3D& dif)
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
	Vector3D operator /(const Vector3D& vec3_scale) const
	{
		return Vector3D(X / vec3_scale.X, Y / vec3_scale.Y, Z / vec3_scale.Z);
	}
	Vector3D& operator /=(const Vector3D& vec3_scale)
	{
		return *this = *this / vec3_scale;
	}
	Vector3D operator -() const
	{
		return Vector3D(-(this->X), -(this->Y), -(this->Z));
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
	Vector3D InterpolatedTo(const Vector3D& point, const Vector3D& end, const float alpha)
	{
		return (*this + (end - *this) * alpha);
	}
	Vector3D InterpolatedTo(const Vector3D& point, const Vector3D& end)
	{
		const float alpha = (point.Y - Y) / (end.Y - Y);
		return (*this + (end - *this) * alpha);
	}
	type Length()
	{
		return (type)sqrt(LengthSq());
	}
	type LengthSq()
	{
		return type(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
	Vector3D Normalized()
	{
		const type len = Length();
		if (len != (type)0.0f) {
			return *this * ((type)1.0f / len);
		}
		return { (type)0,(type)0 };
	}
	Vector3D& Normalize()
	{
		return *this = Normalized();
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
	const type& operator [](int i) const
	{
		i = i % 3;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else if (i == 2) {
			return Z;
		}
		return X;
	}
	type& operator [](int i)
	{
		i = i % 3;
		if (i == 0) {
			return X;
		}
		else if (i == 1) {
			return Y;
		}
		else if (i == 2) {
			return Z;
		}
		return X;
	}
};

using fVector3D = Vector3D<float>;
using dVector3D = Vector3D<double>;
using iVector3D = Vector3D<int>;


/*

	TextureVector

*/


template <typename type>
class TextureVector
{
public:
	Vector3D<type> pos;
	Vector2D<type> tpos;
public:
	TextureVector() = default;
	TextureVector(type x, type y, type z, type tx, type ty)
		:
		pos(x,y,z),
		tpos(tx,ty)
	{}
	TextureVector(Vector3D<type> pos, Vector2D<type> tpos)
		:
		pos(pos),
		tpos(tpos)
	{}
	template <typename vType>
	TextureVector(Vector2D<vType> vec2)
		:
		pos(vec2),
		tpos(type(1),type(1))
	{}
	template <typename vType>
	TextureVector(Vector3D<vType> vec3)
		:
		pos(vec3),
		tpos(type(1),type(0))
	{}
	TextureVector operator +(const TextureVector& tvec) const
	{
		return TextureVector(pos + tvec.pos, tpos + tvec.pos);
	}
	TextureVector& operator +=(const TextureVector& tvec)
	{
		return *this = *this + tvec;
	}
	TextureVector operator -(const TextureVector& tvec) const
	{
		return TextureVector(pos - tvec.pos, tpos - tvec.tpos);
	}
	TextureVector& operator -=(const TextureVector& tvec)
	{
		return *this = *this - tvec;
	}
	TextureVector operator *(const type scale) const
	{
		return TextureVector(pos * scale, tpos * scale);
	}
	TextureVector& operator *=(const type scale)
	{
		return *this = *this * scale;
	}
	TextureVector operator *(const TextureVector& tvec_scale) const
	{
		return TextureVector(pos * tvec_scale.pos, tpos * tvec_scale.tpos);
	}
	TextureVector& operator *=(const TextureVector& tvec_scale)
	{
		return *this = *this * tvec_scale;
	}
	TextureVector operator /(const type scale) const
	{
		return TextureVector(pos / scale, tpos / scale);
	}
	TextureVector& operator /=(const type scale)
	{
		return *this = *this / scale;
	}
	TextureVector operator /(const TextureVector& tvec_scale) const
	{
		return TextureVector(pos / tvec_scale.pos, tpos / tvec_scale.tpos);
	}
	TextureVector& operator /=(const TextureVector& tvec_scale)
	{
		return *this = *this / tvec_scale;
	}
	TextureVector InterpolatedTo(const TextureVector& point, const TextureVector& end) const
	{
		auto pos = this->pos;
		auto tpos = this->tpos;
		return TextureVector(
			pos.InterpolatedTo(point.pos, end.pos),
			tpos.InterpolatedTo(point.tpos, end.tpos)
		);
	}
	TextureVector InterpolatedTo(const TextureVector& point, const TextureVector& end, const float alpha) const
	{
		return TextureVector(
			pos.InterpolatedTo(point.pos, end.pos, alpha),
			tpos.InterpolatedTo(point.tpos, end.tpos, alpha)
		);
	}
};

using fTextureVector = TextureVector<float>;
using dTextureVector = TextureVector<double>;
using iTextureVector = TextureVector<int>;
