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
		tc.reserve( 8 );
		verts.emplace_back( -side,-side,-side );
		tc.emplace_back( 0.0f,1.0f );
		verts.emplace_back( side,-side,-side );
		tc.emplace_back( 1.0f,1.0f );
		verts.emplace_back( side,side,-side );
		tc.emplace_back( 1.0f,0.0f );
		verts.emplace_back( -side,side,-side );
		tc.emplace_back( 0.0f,0.0f );
		verts.emplace_back( -side,side,side );
		tc.emplace_back( 1.0f,0.0f );
		verts.emplace_back( side,side,side );
		tc.emplace_back( 0.0f,0.0f );
		verts.emplace_back( side,-side,side );
		tc.emplace_back( 0.0f,1.0f );
		verts.emplace_back( -side,-side,side );
		tc.emplace_back( 1.0f,1.0f );
	}

	IndexedLineList GetLineList() const
	{
		std::vector<size_t> indices = {
			0,2,1, 0,3,2,
			7,4,3, 0,7,3,
			0,1,6, 0,6,7,
			1,2,5, 1,5,6,
			2,3,4, 2,4,5,
			6,5,4, 6,4,7
		};
		return { verts,std::move( indices ) };
	}

	IndexedTriangleList<Vec3> GetTriangleList() const
	{
		std::vector<size_t> indices = {
			0,2,1, 0,3,2, 
			7,4,3, 0,7,3,
			0,1,6, 0,6,7,
			1,2,5, 1,5,6,
			2,3,4, 2,4,5,
			6,5,4, 6,4,7
		};
		return { verts,std::move( indices ) };
	}

	IndexedTriangleList<TexVertex> GetTriangleTexList() const
	{
		std::vector<TexVertex> tverts;
		tverts.reserve( verts.size() );
		for ( size_t i = 0; i < verts.size(); ++i )
		{
			tverts.emplace_back( verts[i],tc[i] );
		}

		std::vector<size_t> indices = {
			0,2,1, 0,3,2,
			7,4,3, 0,7,3,
			0,1,6, 0,6,7,
			1,2,5, 1,5,6,
			2,3,4, 2,4,5,
			6,5,4, 6,4,7
		};

		return { std::move( tverts ),std::move( indices ) };
	}


private:
	std::vector<Vec3> verts;
	std::vector<Vec2> tc;
};
