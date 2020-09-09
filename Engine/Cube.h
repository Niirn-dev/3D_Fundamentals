#pragma once

#include "Vec3.h"
#include <vector>
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"

class Cube
{
public:
	Cube( float size )
	{
		const float side = size / 2.0f;

		verts.reserve( 8 );
		verts.emplace_back( -side,-side,-side );
		verts.emplace_back( side,-side,-side );
		verts.emplace_back( side,side,-side );
		verts.emplace_back( -side,side,-side );
		verts.emplace_back( -side,side,side );
		verts.emplace_back( side,side,side );
		verts.emplace_back( side,-side,side );
		verts.emplace_back( -side,-side,side );
	}

	IndexedLineList GetLineList() const
	{
		std::vector<size_t> indices = {
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7, 7,4,
			3,4, 2,5, 1,6, 0,7
		};
		return { verts,indices };
	}

	IndexedTriangleList GetTriangleList() const
	{
		std::vector<size_t> indices = {
			0,1,2, 0,2,3, 0,3,4, 0,7,4, 0,1,6, 0,7,6,
			5,4,3, 5,2,3, 5,4,7, 5,6,7, 5,2,1, 5,6,1
		};
		return { verts,indices };
	}


private:
	std::vector<Vec3> verts;
};
