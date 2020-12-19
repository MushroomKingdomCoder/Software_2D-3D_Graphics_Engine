#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Triangle.h"
#include "Vector.h"

template <typename type>
inline type sq(type x)
{
	return x * x;
}

template <typename type>
inline type angle_wrap(type radians)
{
	while (radians > M_PI) {
		radians -= 2 * M_PI;
	}
	return radians;
}

template <typename type>
inline Vector2D<type> vec2_abs(const Vector2D<type>& vec2)
{
	return Vector2D<type>(abs(vec2.X), abs(vec2.Y));
}

template <typename type>
inline Vector3D<type> vec3_abs(const Vector3D<type>& vec3)
{
	return Vector3D<type>(abs(vec3.X), abs(vec3.Y), abs(vec3.Z));
}
template <typename type>
inline TextureVector<type> tvec_abs(const TextureVector<type>& tvec)
{
	return TextureVector<type>(vec3_abs(tvec.pos), vec2_abs(tvec.tpos));
}

template <typename vertex>
inline void subdivide(const fVector3D& v1, const fVector3D& v2, const fVector3D& v3, std::vector<vertex>& sphere_points, std::vector<Triangle<int>>& triangles, const unsigned int depth) {
    if (depth == 0) {
        sphere_points.emplace_back(v1);
        sphere_points.emplace_back(v2);
        sphere_points.emplace_back(v3);
        unsigned int next_index = unsigned int(sphere_points.size());
        triangles.emplace_back(next_index - 2, next_index - 3, next_index - 1);
        return;
    }
    const fVector3D v12 = (v1 + v2).Normalized();
    const fVector3D v23 = (v2 + v3).Normalized();
    const fVector3D v31 = (v3 + v1).Normalized();
    const unsigned int next_depth = depth - 1;
    subdivide(v1, v12, v31, sphere_points, triangles, next_depth);
    subdivide(v2, v23, v12, sphere_points, triangles, next_depth);
    subdivide(v3, v31, v23, sphere_points, triangles, next_depth);
    subdivide(v12, v23, v31, sphere_points, triangles, next_depth);
}

template <typename vertex>
inline void initialize_sphere(std::vector<vertex>& sphere_points, std::vector<Triangle<int>>& triangles, const unsigned int depth) {
    const double X = 0.525731112119133606;
    const double Z = 0.850650808352039932;
    const std::vector<fVector3D> vdata = {
        {-X, 0.0, Z}, { X, 0.0, Z }, { -X, 0.0, -Z }, { X, 0.0, -Z },
        { 0.0, Z, X }, { 0.0, Z, -X }, { 0.0, -Z, X }, { 0.0, -Z, -X },
        { Z, X, 0.0 }, { -Z, X, 0.0 }, { Z, -X, 0.0 }, { -Z, -X, 0.0 }
    };
    std::vector<Triangle<int>> tindices = {
        { 0, 4, 1 }, { 0, 9, 4 }, { 9, 5, 4 }, { 4, 5, 8 }, { 4, 8, 1 },
        { 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 }, { 5, 2, 3 }, { 2, 7, 3 },
        { 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },
        { 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11 }
    };
    for (int i = 0; i < 20; i++) {
        subdivide(vdata[tindices[i].v0], vdata[tindices[i].v1], vdata[tindices[i].v2], sphere_points, triangles, depth);
    }
}