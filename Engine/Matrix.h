#pragma once
#include "Vector.h"

/*

	Matrix 2D

*/


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
				r_mtx.Cell[lrow][rcol] = type(0);
				for (int i = 0; i < 2; ++i) {
					r_mtx.Cell[lrow][rcol] += Cell[lrow][i] * mtx.Cell[i][rcol];
				}
			}
		}
		return r_mtx;
	}
	Matrix2D& operator *=(const Matrix2D& mtx)
	{
		return *this = *this * mtx;
	}
	static Matrix2D Scale(const type& scl)
	{
		return Matrix2D{
			scl,		type(0),
			type(0),	scl
		};
	}
	static Matrix2D Identity()
	{
		return Scale(type(1));
	}
	static Matrix2D FlipY()
	{
		return Matrix2D{
			type(1),	type(0),
			type(0),	type(-1)
		};
	}
	static Matrix2D Rotation(const type& radians)
	{
		const auto cosR = type(cos(radians));
		const auto sinR = type(sin(radians));
		return Matrix2D{
			cosR,	-sinR,
			sinR,	cosR
		};
	}
};

using fMatrix2D = Matrix2D<float>;
using dMatrix2D = Matrix2D<double>;
using iMatrix2D = Matrix2D<int>;


/*

	Matrix2Dplus

*/


template <typename type>
class Matrix2Dplus
{
public:
	type Cell[3][3];

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
	Vector2D<type> operator *(const Vector2D<type> vec) const
	{
		return Vector2D<type>(*this * Vector3D<type>(vec));
	}
	Matrix2Dplus operator *(const Matrix2Dplus& mtx) const
	{
		Matrix2Dplus r_mtx;
		for (int lrow = 0; lrow < 3; ++lrow) {
			for (int rcol = 0; rcol < 3; ++rcol) {
				r_mtx.Cell[lrow][rcol] = type(0);
				for (int i = 0; i < 3; ++i) {
					r_mtx.Cell[lrow][rcol] += Cell[lrow][i] * mtx.Cell[i][rcol];
				}
			}
		}
		return r_mtx;
	}
	static Matrix2Dplus Scale(const type& scl)
	{
		return Matrix2Dplus{
			scl,		type(0),	type(0),
			type(0),	scl,		type(0),
			type(0),	type(0),	type(1)
		};
	}
	static Matrix2Dplus Identity()
	{
		return Scale(type(1));
	}
	static Matrix2Dplus FlipY()
	{
		return Matrix2Dplus{
			type(1),	type(0),	type(0),
			type(0),	type(-1),	type(0),
			type(0),	type(0),	type(1)
		};
	}
	static Matrix2Dplus Rotation(const type& radians)
	{
		const auto cosR = type(cos(radians));
		const auto sinR = type(sin(radians));
		return Matrix2Dplus{
			cosR, -sinR, type(0),
			sinR, cosR, type(0),
			type(0), type(0), type(1)
		};
	}
	static Matrix2Dplus Translation(const type x, const type y)
	{
		return Matrix2Dplus{
			type(1),	type(0),	x,
			type(0),	type(1),	y,
			type(0),	type(0),	type(1)
		};
	}
	static Matrix2Dplus Translation(const Vector2D<type>& tvec)
	{
		return Matrix2Dplus{
			type(1),	type(0),	tvec.X,
			type(0),	type(1),	tvec.Y,
			type(0),	type(0),	type(1)
		};
	}
};

using fMatrix2Dplus = Matrix2Dplus<float>;
using dMatrix2Dplus = Matrix2Dplus<double>;
using iMatrix2Dplus = Matrix2Dplus<int>;


/*

	Matrix3D

*/


template <typename type>
class Matrix3D
{
public:
	type Cell[3][3];

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
				r_mtx.Cell[lrow][rcol] = type(0);
				for (int i = 0; i < 3; ++i) {
					r_mtx.Cell[lrow][rcol] += Cell[lrow][i] * mtx.Cell[i][rcol];
				}
			}
		}
		return r_mtx;
	}
	Matrix3D& operator *=(const Matrix3D& mtx)
	{
		return *this = *this * mtx;
	}
	static Matrix3D Identity()
	{
		return Scale(type(1));
	}
	static Matrix3D Scale(const type& scl)
	{
		return Matrix3D{
			scl,		type(0),	type(0),
			type(0),	scl,		type(0),
			type(0),	type(0),	scl
		};
	}
	static Matrix3D ScaleIndependent(const type scl_x, const type scl_y, const type scl_z)
	{
		return Matrix3D{
			scl_x,		type(0),	type(0),
			type(0),	scl_y,		type(0),
			type(0),	type(0),	scl_z
		};
	}
	static Matrix3D RotationX(const float radians)
	{
		const float cosR = (float)cos(radians);
		const float sinR = (float)sin(radians);
		return Matrix3D{
			type(1),	type(0),	type(0),
			type(0),	cosR,		-sinR,
			type(0),	sinR,		cosR
		};
	}
	static Matrix3D RotationY(const float radians)
	{
		const float cosR = (float)cos(radians);
		const float sinR = (float)sin(radians);
		return Matrix3D{
			cosR,		type(0),	sinR,
			type(0),	type(1),	type(0),
			-sinR,		type(0),	cosR
		};
	}
	static Matrix3D RotationZ(const float radians)
	{
		const float cosR = (float)cos(radians);
		const float sinR = (float)sin(radians);
		return Matrix3D{
			cosR,		-sinR,		type(0),
			sinR,		cosR,		type(0),
			type(0),	type(0),	type(1)
		};
	}
};

using fMatrix3D = Matrix3D<float>;
using dMatrix3D = Matrix3D<double>;
using iMatrix3D = Matrix3D<int>;