#pragma once
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

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

	Vector2D operator-(const Vector2D& dif) const
	{
		return { X - dif.X, Y - dif.Y };
	}

	Vector2D& operator-=(const Vector2D& dif)
	{
		return *this = *this - dif;
	}

	Vector2D operator -() const
	{
		return Vector2D(-(this->X), -(this->Y));
	}

	type operator *(const Vector2D& vec) const
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
