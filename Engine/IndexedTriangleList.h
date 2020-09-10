#pragma once

#include <vector>
#include "Vec3.h"
#include <cassert>

struct IndexedTriangleList
{
	IndexedTriangleList( std::vector<Vec3> vertices,std::vector<size_t> indices )
		:
		vertices( std::move( vertices ) ),
		indices( std::move( indices ) )
	{
		assert( this->vertices.size() > 2 );
		assert( this->indices.size() % 3 == 0 );
		cullingMask.resize( this->indices.size() / 3,true );
	}

	std::vector<Vec3> vertices;
	std::vector<size_t> indices;
	std::vector<bool> cullingMask;
};
