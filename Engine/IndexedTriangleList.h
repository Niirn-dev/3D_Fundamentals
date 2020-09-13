#pragma once

#include <vector>
#include "Vec3.h"
#include <cassert>
#include <algorithm>
#include <iterator>

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
	IndexedTriangleList& operator+=( const IndexedTriangleList& rhs )
	{
		const size_t nVerts = vertices.size();
		std::copy( rhs.vertices.begin(),rhs.vertices.end(),std::back_inserter( vertices ) );
		std::transform( rhs.indices.begin(),rhs.indices.end(),
						std::back_inserter( indices ),
						[&nVerts]( size_t i )
						{
							return i + nVerts;
						} );
		return *this;
	}
	IndexedTriangleList operator+( const IndexedTriangleList& rhs ) const
	{
		return IndexedTriangleList( *this ) += rhs;
	}

	std::vector<T> vertices;
	std::vector<size_t> indices;
};
