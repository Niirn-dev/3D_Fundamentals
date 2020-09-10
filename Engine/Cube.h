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
			0,2,1, 0,3,2, 
			7,4,3, 0,7,3,
			0,1,6, 0,6,7,
			1,2,5, 1,5,6,
			2,3,4, 2,4,5,
			6,5,4, 6,4,7
		};
		return { verts,indices };
	}


private:
	std::vector<Vec3> verts;
};
