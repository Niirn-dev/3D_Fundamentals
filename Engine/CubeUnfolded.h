#pragma once

#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "TexVertex.h"

class CubeUnfolded
{
public:
	CubeUnfolded( float size )
	{
		const float side = size / 2.0f;

		verts.reserve( 14 );
		tc.reserve( 14 );

		verts.emplace_back( -side,side,-side ); // 0
		tc.emplace_back( 0.0f,0.0f );
		verts.emplace_back( side,side,-side ); // 1
		tc.emplace_back( 1.0f,0.0f );
		verts.emplace_back( -side,-side,-side ); // 2
		tc.emplace_back( 0.0f,1.0f );
		verts.emplace_back( side,-side,-side ); // 3
		tc.emplace_back( 1.0f,1.0f );

		verts.emplace_back( side,-side,side ); // 4
		tc.emplace_back( 2.0f,1.0f );
		verts.emplace_back( side,side,side ); // 5
		tc.emplace_back( 2.0f,0.0f );

		verts.emplace_back( -side,side,side ); // 6
		tc.emplace_back( 3.0f,0.0f );
		verts.emplace_back( -side,-side,side ); // 7
		tc.emplace_back( 3.0f,1.0f );

		verts.emplace_back( -side,-side,-side ); // 8
		tc.emplace_back( 4.0f,1.0f );
		verts.emplace_back( -side,side,-side ); // 9
		tc.emplace_back( 4.0f,0.0f );

		verts.emplace_back( -side,side,-side ); // 10
		tc.emplace_back( 1.0f,-1.0f );
		verts.emplace_back( -side,side,side ); // 11
		tc.emplace_back( 2.0f,-1.0f );

		verts.emplace_back( -side,-side,-side ); // 12
		tc.emplace_back( 1.0f,2.0f );
		verts.emplace_back( -side,-side,side ); // 13
		tc.emplace_back( 2.0f,2.0f );
	};

	inline IndexedLineList GetLineList() const = delete;
	/*{
		std::vector<size_t> indices = {
			0,2,1, 0,3,2,
			7,4,3, 0,7,3,
			0,1,6, 0,6,7,
			1,2,5, 1,5,6,
			2,3,4, 2,4,5,
			6,5,4, 6,4,7
		};
		return { verts,std::move( indices ) };
	}*/

	inline IndexedTriangleList<Vec3> GetTriangleList() const
	{
		std::vector<size_t> indices = {
			0,1,2,   2,1,3,
			1,4,3,   4,1,5,
			5,6,4,   4,6,7,
			6,8,7,   8,6,9,
			1,10,11, 11,5,1,
			4,13,12, 12,3,4
		};
		return { verts,std::move( indices ) };
	}

	inline IndexedTriangleList<TexVertex> GetTriangleTexList() const
	{
		std::vector<TexVertex> tverts;
		tverts.reserve( verts.size() );
		for ( size_t i = 0; i < verts.size(); ++i )
		{
			tverts.emplace_back( verts[i],tc[i] );
		}

		std::vector<size_t> indices = {
			0,1,2,   2,1,3,
			1,4,3,   4,1,5,
			5,6,4,   4,6,7,
			6,8,7,   8,6,9,
			1,10,11, 11,5,1,
			4,13,12, 12,3,4
		};

		return { std::move( tverts ),std::move( indices ) };
	}

private:
	std::vector<Vec3> verts;
	std::vector<Vec2> tc;
};
