#pragma once

#include "IndexedTriangleList.h"
#include <vector>
#include <array>
#include "ChiliMath.h"

class Plane
{
public:
	template<class V>
	static IndexedTriangleList<V> MakePlain( float width,float height,int divide_x = 7,int divide_y = 7 )
	{
		// number of vertices considering tessellation
		const int nVertices_x = divide_x + 1;
		const int nVertices_y = divide_y + 1;

		// distance between vertices
		const float step_x = width / (float)divide_x;
		const float step_y = height / (float)divide_y;

		// set up vertices of the tessallated plane
		std::vector<V> vertices;
		vertices.reserve( (size_t)nVertices_x * nVertices_y );
		const Vec3 bottomLeft = { -(float)width / 2.0f,-(float)height / 2.0f,0.0f };
		for ( int y = 0; y < nVertices_y; ++y )
		{
			const float offset_y = y * step_y;
			for ( int x = 0; x < nVertices_x; ++x )
			{
				vertices.emplace_back( bottomLeft + Vec3{ x * step_x,offset_y,0.0f } );
			}
		}

		// set up vertex list for the triangles
		std::vector<size_t> indices;
		indices.reserve( (size_t)divide_x * divide_y * 6 );
		{
			const auto vxy2i = [nVertices_x]( size_t x,size_t y )
			{
				return y * nVertices_x + x;
			};

			// iterate over bottom left vertices of all quads on the plane
			for ( size_t y = 0; y < divide_y; ++y )
			{
				for ( size_t x = 0; x < divide_y; ++x )
				{
					// vertices of the current quad
					const std::array<size_t,4> cornerIndices = {
						vxy2i( x,y ),vxy2i( x + 1,y ),vxy2i( x,y + 1 ),vxy2i( x + 1,y + 1 )
					};
					// make forward facing triangles
					indices.push_back( cornerIndices[0] );
					indices.push_back( cornerIndices[2] );
					indices.push_back( cornerIndices[1] );
					indices.push_back( cornerIndices[1] );
					indices.push_back( cornerIndices[2] );
					indices.push_back( cornerIndices[3] );
				}
			}
		}

		return { std::move( vertices ),std::move( indices ) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeDoublePlane( float width,float height,int divide_x = 7,int divide_y = 7 )
	{
		// get single plane
		auto itlist = Plane::MakePlain<V>( width,height,divide_x,divide_y );

		// add indices for back facing triangles
		itlist.indices.reserve( sq( (size_t)divide_x * divide_y ) * 6 );
		{
			const int nVertices_x = divide_x + 1;
			const int nVertices_y = divide_y + 1;

			const auto vxy2i = [nVertices_x]( size_t x,size_t y )
			{
				return y * nVertices_x + x;
			};

			// iterate over bottom left vertices of all quads on the plane
			for ( size_t y = 0; y < divide_y; ++y )
			{
				for ( size_t x = 0; x < divide_y; ++x )
				{
					// vertices of the current quad
					const std::array<size_t,4> cornerIndices = {
						vxy2i( x,y ),vxy2i( x + 1,y ),vxy2i( x,y + 1 ),vxy2i( x + 1,y + 1 )
					};
					// make back facing triangles
					itlist.indices.push_back( cornerIndices[0] );
					itlist.indices.push_back( cornerIndices[1] );
					itlist.indices.push_back( cornerIndices[2] );
					itlist.indices.push_back( cornerIndices[2] );
					itlist.indices.push_back( cornerIndices[1] );
					itlist.indices.push_back( cornerIndices[3] );
				}
			}
		}

		return std::move( itlist );
	}

	template<class V>
	static IndexedTriangleList<V> MakeSkinned( float width,float height,float tScale = 1.0f,int divide_x = 7,int divide_y = 7 )
	{
		// get indexed triangle list for plain plane
		auto itlist = Plane::MakePlain<V>( width,height,divide_x,divide_y );
		// set texture coordinates
		const int nVertices_x = divide_x + 1;
		const int nVertices_y = divide_y + 1;

		const Vec2 tBottomLeft = { 0.0f,1.0f };
		const float tDivisionSize_x = width / (float)divide_x;
		const float tDivisionSize_y = height / (float)divide_y;

		for ( size_t y = 0,i = 0; y < nVertices_y; ++y )
		{
			const float offset_y = y * tDivisionSize_y / tScale;
			for ( size_t x = 0; x < nVertices_x; ++x,++i )
			{
				itlist.vertices[i].tc = tBottomLeft + Vec2{ x * tDivisionSize_x / tScale,offset_y };
			}
		}

		return std::move( itlist );
	}

	template<class V>
	static IndexedTriangleList<V> MakeSkinnedDouble( float width,float height,float tScale = 1.0f,int divide_x = 7,int divide_y = 7 )
	{
		// get indexed triangle list for plain plane
		auto itlist = Plane::MakeDoublePlane<V>( width,height,divide_x,divide_y );
		// set texture coordinates
		const int nVertices_x = divide_x + 1;
		const int nVertices_y = divide_y + 1;

		const Vec2 tBottomLeft = { 0.0f,1.0f };
		const float tDivisionSize_x = width / (float)divide_x;
		const float tDivisionSize_y = height / (float)divide_y;

		for ( size_t y = 0,i = 0; y < nVertices_y; ++y )
		{
			const float offset_y = y * tDivisionSize_y / tScale;
			for ( size_t x = 0; x < nVertices_x; ++x,++i )
			{
				itlist.vertices[i].tc = tBottomLeft + Vec2{ x * tDivisionSize_x / tScale,offset_y };
			}
		}

		return std::move( itlist );
	}
};
