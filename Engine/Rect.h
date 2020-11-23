#pragma once
#include "Vector2D.h"
#include <vector>
#include <algorithm>

template <typename type>
class Rect 
{
public:
	type Top;
	type Right;
	type Bottom;
	type Left;
public:
	Rect() = default;

	Rect(const type left, const type top, const type right, const type bottom)
		:
		Left(left),
		Top(top),
		Right(right),
		Bottom(bottom)
	{}

	Rect(const Vector2D<type>& topleft, const Vector2D<type>& bottomright)
		:
		Rect<type>(topleft.X, topleft.Y, bottomright.X, bottomright.Y)
	{}

	Rect(const Vector2D<type>& topleft, const type width, const type height)
		:
		Rect<type>(topleft, Vector2D<type>(topleft.X + width, topleft.Y - height))
	{}

	Rect(const Vector2D<type>& topleft, const type halfside)
		:
		Rect<type>(topleft, halfside * 2, halfside * 2)
	{}

	Rect(const std::vector<Vector2D<type>>& points)
	{
		Left = 0; Top = 0; Right = 0; Bottom = 0;

		for (auto i = points.begin(); i < points.end(); ++i) {
			auto lesser_x = [&](const Vector2D<type>& point) { return point.X < i->X; };
			auto lesser_y = [&](const Vector2D<type>& point) { return point.Y < i->Y; };
			auto greater_x = [&](const Vector2D<type>& point) { return point.X > i->X; };
			auto greater_y = [&](const Vector2D<type>& point) { return point.Y > i->Y; };

			if (!std::any_of(points.begin(), points.end(), lesser_x)) {
				Left = i->X;
			}
			if (!std::any_of(points.begin(), points.end(), greater_x)) {
				Right = i->X;
			}
			if (!std::any_of(points.begin(), points.end(), greater_y)) {
				Top = i->Y;
			}
			if (!std::any_of(points.begin(), points.end(), lesser_y)) {
				Bottom = i->Y;
			}
		}
	}

	template <typename sType>
	Rect(Rect<sType> S)
		:
		Left((type)S.Left),
		Top((type)S.Top),
		Right((type)S.Right),
		Bottom((type)S.Bottom)
	{}

	void Move(const Vector2D<type>& vel)
	{
		Right += vel.X;
		Left += vel.X;
		Top += vel.Y;
		Bottom += vel.Y;
	}

	Rect Rotated(const float radians)
	{
		const float cosR = cos(radians);
		const float sinR = sin(radians);
		auto topleft = Vector2D<type>(Left, Top).Rotate(cosR, sinR);
		auto topright = Vector2D<type>(Right, Top).Rotate(cosR, sinR);
		auto bottomright = Vector2D<type>(Right, Bottom).Rotate(cosR, sinR);
		auto bottomleft = Vector2D<type>(Left, Bottom).Rotate(cosR, sinR);
		return Rect(std::vector<fVector2D>({ topleft,topright,bottomright,bottomleft }));
	}

	Rect& Rotate(const float radians)
	{
		return *this = this->Rotated(radians);
	}

	bool OverlappedWith(const Rect<type>& other) const
	{
		return
			(Right > other.Left && Bottom < other.Top && Left < other.Right&& Top > other.Bottom);
	}

	template <typename pType>
	bool ContainsPoint(const Vector2D<pType>& point) const
	{
		return (point.X >= Left && point.Y >= Top && point.X < Right && point.Y < Bottom);
	}

	Vector2D<type> GetCenter()
	{
		return Vector2D<type>(Left + (Right - Left) / 2, Top - (Top - Bottom) / 2);
	}

	type GetWidth()
	{
		return type(Right - Left);
	}

	type GetHeight()
	{
		return type(Top - Bottom);
	}

	void Scale(const type scale)
	{
		Left *= scale;
		Top *= scale;
		Right *= scale;
		Bottom *= scale;
	}

	Vector2D<type> GetTopLeft() const
	{
		return Vector2D<type>(Left, Top);
	}

	Vector2D<type> GetTopRight() const
	{
		return Vector2D<type>(Right, Top);
	}

	Vector2D<type> GetBottomRight() const
	{
		return Vector2D<type>(Right, Bottom);
	}

	Vector2D<type> GetBottomLeft() const
	{
		return Vector2D<type>(Left, Bottom);
	}

public:
	static Rect<type> MakeFromCenter(Vector2D<type> center, type halfWidth, type halfHeight)
	{
		return Rect<type>(Vector2D<type>(center.X - halfWidth, center.Y + halfHeight), halfWidth * 2, halfHeight * 2);
	}
};

typedef Rect<int> iRect;
typedef Rect<float> fRect;