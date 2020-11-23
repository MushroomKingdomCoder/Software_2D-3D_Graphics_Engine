#pragma once
#include "Vector3D.h"

template <typename type>
class Matrix3D
{
public:
	type Cell[2][2];

public:
	Vector3D<type> operator *(const Vector3D<type>& vec) const
	{
		Vector3D<type> r_vec = { 0,0 };
		for (int row = 0; row < 3; ++row) {
			type sum = type(0);
			for (int col = 0; col < 3; ++col) {
				sum += Cell[row][col] * vec[col];
			}
			r_vec[row] = sum;
		}
		return r_vec;
	}

	Matrix3D operator *(const Matrix3D& mtx) const
	{
		Matrix3D r_mtx;
		for (int lrow = 0; lrow < 3; ++lrow) {
			for (int rcol = 0; rcol < 3; ++rcol) {
				r_mtx[lrow][rcol] = type(0);
				for (int i = 0; i < 3; ++i) {
					r_mtx[lrow][rcol] += Cell[lrow][i] * mtx.Cell[i][rcol];
				}
			}
		}
		return r_mtx;
	}

	Matrix3D& operator *(const Matrix3D& mtx)
	{
		return *this = mtx * *this;
	}

	static Matrix3D Scale(const type& scl)
	{
		return Matrix3D(
			scl,		type(0),	type(0),
			type(0),	scl,		type(0),
			type(0),	type(0),	type(1)
		);
	}

	static Matrix3D Identity()
	{
		return Scale(type(1));
	}

	static Matrix3D FlipY()
	{
		return Matrix3D(
			type(1),	type(0),	type(0),
			type(0),	type(-1),	type(0),
			type(0),	type(0),	type(1)
		);
	}

	static Matrix3D Rotation(const type& radians)
	{
		const auto cosR = type(cos(radians));
		const auto sinR = type(sin(radians));
		return Matrix3D(
			cosR,		-sinR,		type(0),
			sinR,		cosR,		type(0),
			type(0),	type(0),	type(1)
		);
	}

	static Matrix3D Translation(const type x, const type y)
	{
		return Matrix3D(
			type(1),	type(0),	x,
			type(0),	type(1),	y,
			type(0),	type(0),	type(1)
		);
	}
};

using fMatrix3D = Matrix3D<float>;
using dMatrix3D = Matrix3D<double>;
using iMatrix3D = Matrix3D<int>;