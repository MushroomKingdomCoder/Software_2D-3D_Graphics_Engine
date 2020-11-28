#pragma once
#include "TriangleIndexBuffer.h"
#include "VertexIndexBuffer.h"
#include "Vector.h"
#include <vector>

class Prism
{
private:
	std::vector<fVector3D> Verticies;
	fVector3D Position;

public:
	Prism(std::vector<fVector3D> verts, fVector3D pos = { 0,0,0 })
		:
		Verticies(verts)
	{}
	VertexIndexBuffer GetVIB() const
	{
		return VertexIndexBuffer{
			Verticies, {
			{0,1},	{0,3},	{0,4},
			{2,1},	{2,3},	{2,6},
			{5,4},	{5,6},	{5,1},
			{7,6},	{7,4},	{7,3}
		} };
	}
	TriangleIndexBuffer GetTIB() const
	{
		return TriangleIndexBuffer{
			Verticies, {
			{0,1,3},	{0,1,4},	{0,3,4},
			{2,1,3},	{2,1,6},	{2,3,6},
			{5,4,1},	{5,6,1},	{5,4,6},
			{7,4,3},	{7,6,3},	{7,4,6}
		} };
	}
public:
	static Prism MakeCube(float size, fVector3D pos = { 0,0,0 })
	{
		auto s = size / 2.0f;
		return Prism({
			{-s,-s,-s},
			{-s,-s,s},
			{-s,s,s},
			{-s,s,-s},
			{s,-s,-s},
			{s,-s,s},
			{s,s,s},
			{s,s,-s}
		}, pos);
	}
};