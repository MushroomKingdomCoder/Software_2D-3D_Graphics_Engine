#pragma once
#include "Vector2D.h"

template <typename type>
class Matrix2D
{
public:
	type Cell[2][2];

public:
	Vector2D<type> operator *(const Vector2D<type>& vec) const
	{
		Vector2D<type> r_vec = { 0,0 };
		for (int row = 0; row < 2; ++row) {
			type sum = type(0);
			for (int col = 0; col < 2; ++col) {
				sum += Cell[row][col] * vec[col];
			}
			r_vec[row] = sum;
		}
		return r_vec;
	}

	Matrix2D operator *(const Matrix2D& mtx) const
	{
		Matrix2D r_mtx;
		for (int lrow = 0; lrow < 2; ++lrow) {
			for (int rcol = 0; rcol < 2; ++rcol) {
				r_mtx[lrow][rcol] = type(0);
				for (int i = 0; i < 2; ++i) {
					r_mtx[lrow][rcol] += Cell[lrow][i] * mtx.Cell[i][rcol];
				}
			}
		}
		return r_mtx;
	}

	Matrix2D& operator *(const Matrix2D& mtx)
	{
		return *this = *this * mtx;
	}

	static Matrix2D Scale(const type& scl)
	{
		return Matrix2D(
			scl,		type(0),
			type(0),	scl 
		);
	}

	static Matrix2D Identity()
	{
		return Scale(type(1));
	}

	static Matrix2D FlipY()
	{
		return Matrix2D(
			type(1),	type(0),
			type(0),	type(-1)
		);
	}

	static Matrix2D Rotation(const type& radians)
	{
		const auto cosR = type(cos(radians));
		const auto sinR = type(sin(radians));
		return Matrix2D(
			cosR,	-sinR,
			sinR,	cosR
		);
	}
};

using fMatrix2D = Matrix2D<float>;
using dMatrix2D = Matrix2D<double>;
using iMatrix2D = Matrix2D<int>;