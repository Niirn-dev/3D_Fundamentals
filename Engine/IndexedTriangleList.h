#pragma once

#include <vector>
#include "Vec3.h"
#include <cassert>

template<typename T>
class IndexedTriangleList
{
public:
	IndexedTriangleList( std::vector<T> vertices,std::vector<size_t> indices )
		:
		vertices( std::move( vertices ) ),
		indices( std::move( indices ) )
	{
		assert( this->vertices.size() > 2 );
		assert( this->indices.size() % 3 == 0 );
	}

	std::vector<T> vertices;
	std::vector<size_t> indices;
};
