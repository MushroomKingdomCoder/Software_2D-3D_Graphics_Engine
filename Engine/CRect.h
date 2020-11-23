#pragma once
#include "Vector2D.h"

template <typename type>
class CRect
{
public:
	type Top;
	type Right;
	type Bottom;
	type Left;
public:
	CRect() = default;

	CRect(const type left, const type top, const type right, const type bottom)
		:
		Left(left),
		Top(top),
		Right(right),
		Bottom(bottom)
	{}

	CRect(const Vector2D<type> & topleft, const Vector2D<type> & bottomright)
		:
		CRect<type>(topleft.X, topleft.Y, bottomright.X, bottomright.Y)
	{}

	CRect(const Vector2D<type> & topleft, const type width, const type height)
		:
		CRect<type>(topleft, Vector2D<type>(topleft.X + width, topleft.Y + height))
	{}

	CRect(const Vector2D<type> & topleft, const type halfside)
		:
		CRect<type>(topleft, halfside * 2, halfside * 2)
	{}

	CRect(const std::vector<Vector2D<type>> & points)
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
			if (!std::any_of(points.begin(), points.end(), lesser_y)) {
				Top = i->Y;
			}
			if (!std::any_of(points.begin(), points.end(), greater_y)) {
				Bottom = i->Y;
			}
		}
	}

	template <typename sType>
	CRect(CRect<sType> S)
		:
		Left((type)S.Left),
		Top((type)S.Top),
		Right((type)S.Right),
		Bottom((type)S.Bottom)
	{}

	void Move(const Vector2D<type> & vel)
	{
		Right += vel.X;
		Left += vel.X;
		Top += vel.Y;
		Bottom += vel.Y;
	}

	bool OverlappedWith(const CRect<type> & other) const
	{
		return
			(Right >= other.Left && Bottom >= other.Top && Left <= other.Right && Top <= other.Bottom);
	}

	template <typename pType>
	bool ContainsPoint(const Vector2D<pType> & point) const
	{
		return 
			(point.X >= Left && point.Y >= Top && point.X < Right && point.Y < Bottom);
	}

	Vector2D<type> GetCenter()
	{
		return Vector2D<type>(Left + (Right - Left) / 2, Top + (Bottom - Top) / 2);
	}

	type GetWidth()
	{
		return type(Right - Left);
	}

	type GetHeight()
	{
		return type(Bottom - Top);
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
	static CRect<type> MakeFromCenter(Vector2D<type> center, type halfWidth, type halfHeight)
	{
		return CRect<type>(Vector2D<type>(center.X - halfWidth, center.Y - halfHeight), halfWidth * 2, halfHeight * 2);
	}
};

typedef CRect<int> iCRect;
typedef CRect<float> fCRect;