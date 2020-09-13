#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Mat3.h"
#include <vector>
#include "IndexedTriangleList.h"
#include "Triangle.h"
#include "Graphics.h"
#include "PubeScreenTransformer.h"
#include "Surface.h"
#include <string>
#include <memory>
#include "ChiliMath.h"
#include <cmath>

template<class Effect>
class Pipeline
{
public:
	using Vertex = typename Effect::Vertex;
public:
	Pipeline( Graphics& gfx )
		:
		gfx( gfx )
	{
	}

	void BindRotation( const Mat3& rot )
	{
		rotation = rot;
	}
	void BindTranslation( const Vec3& trans )
	{
		translation = trans;
	}
	void Draw( const IndexedTriangleList<Vertex>& list )
	{
		ProcessVertices( list.vertices,list.indices );
	}

private:
	// Vertex Transformer
	void ProcessVertices( const std::vector<Vertex>& verts,const std::vector<size_t>& indices )
	{
		std::vector<Vertex> verticesOut;
		verticesOut.reserve( verts.size() );
		for ( const auto& v : verts )
		{
			verticesOut.emplace_back( v.pos * rotation + translation,v );
		}
		AssembleTriangles( verticesOut,indices );
	}

	// Triangle Assembler
	void AssembleTriangles( const std::vector<Vertex>& verts,const std::vector<size_t>& indices )
	{
		for ( size_t i = 0ull; i < indices.size() / 3; ++i )
		{
			const Vertex& v0 = verts[indices[i * 3]];
			const Vertex& v1 = verts[indices[i * 3 + 1]];
			const Vertex& v2 = verts[indices[i * 3 + 2]];

			if ( ( v1.pos - v0.pos ) % ( v2.pos - v0.pos ) * v0.pos < 0.0f )
			{
				ProcessTriangle( v0,v1,v2 );
			}
		}
	}
	void ProcessTriangle( const Vertex& v0,const Vertex& v1,const Vertex& v2 )
	{
		ProcessTriangleVertices( Triangle<Vertex>{ v0,v1,v2 } );
	}

	// Perspective/Screen Transformer
	void ProcessTriangleVertices( Triangle<Vertex>& triangle )
	{
		pst.Transform( triangle.v0.pos );
		pst.Transform( triangle.v1.pos );
		pst.Transform( triangle.v2.pos );

		DrawTriangle( triangle );
	}

	// Triangle Rasterizer
	void DrawTriangle( const Triangle<Vertex>& triangle )
	{
		// Get pointers to verts for easier swapping
		const auto* pv0 = &triangle.v0;
		const auto* pv1 = &triangle.v1;
		const auto* pv2 = &triangle.v2;

		// Sort verts by y
		if ( pv0->pos.y > pv1->pos.y ) std::swap( pv0,pv1 );
		if ( pv0->pos.y > pv2->pos.y ) std::swap( pv0,pv2 );
		if ( pv1->pos.y > pv2->pos.y ) std::swap( pv1,pv2 );

		// Establish the type of the passed in triangle
		if ( pv0->pos.y == pv1->pos.y ) // Flat top triangle
		{
			// Sort verts by x
			if ( pv0->pos.x > pv1->pos.x ) std::swap( pv0,pv1 );
			DrawTriangleFlatTop( *pv0,*pv1,*pv2 );
		}
		else if ( pv1->pos.y == pv2->pos.y ) // Flat bottom triangle
		{
			// Sort verts by x
			if ( pv1->pos.x > pv2->pos.x ) std::swap( pv1,pv2 );
			DrawTriangleFlatBottom( *pv0,*pv1,*pv2 );
		}
		else // General triangle
		{
			// Find splitting vertex
			const float viAlpha =
				( pv1->pos.y - pv0->pos.y ) /
				( pv2->pos.y - pv0->pos.y );
			const auto vi = interpolate( *pv0,*pv2,viAlpha );

			if ( vi.pos.x > pv1->pos.x ) // Major right triangle
			{
				DrawTriangleFlatBottom( *pv0,*pv1,vi );
				DrawTriangleFlatTop( *pv1,vi,*pv2 );
			}
			else // Major left triangle
			{
				DrawTriangleFlatBottom( *pv0,vi,*pv1 );
				DrawTriangleFlatTop( vi,*pv1,*pv2 );
			}
		}
	}
	void DrawTriangleFlatTop( const Vertex& v0,
							  const Vertex& v1,
							  const Vertex& v2 )
	{
		// Get dVertex / dy
		const float dy = v2.pos.y - v0.pos.y;
		const auto dv0 = ( v2 - v0 ) / dy;
		const auto dv1 = ( v2 - v1 ) / dy;

		// Get interpolated edges
		auto itEdge1 = v1;

		DrawTriangleFlat( v0,v1,v2,dv0,dv1,itEdge1 );
	}
	void DrawTriangleFlatBottom( const Vertex& v0,
								 const Vertex& v1,
								 const Vertex& v2 )
	{
		// Get dVertex / dy
		const float dy = v2.pos.y - v0.pos.y;
		const auto dv0 = ( v1 - v0 ) / dy;
		const auto dv1 = ( v2 - v0 ) / dy;

		// Get the interpolated edges
		auto itEdge1 = v0;

		DrawTriangleFlat( v0,v1,v2,dv0,dv1,itEdge1 );
	}
	void DrawTriangleFlat( const Vertex& v0,
						   const Vertex& v1,
						   const Vertex& v2,
						   const Vertex& dv0,
						   const Vertex& dv1,
						   Vertex& itEdge1 )
	{
		// Get the interpolated edges
		auto itEdge0 = v0;

		// Get the start and end points for y according to top rule
		const int yStart = (int)std::ceil( v0.pos.y - 0.5f );
		const int yEnd = (int)std::ceil( v2.pos.y - 0.5f );

		// Prestep into texture
		itEdge0 += dv0 * ( (float)yStart + 0.5f - v0.pos.y );
		itEdge1 += dv1 * ( (float)yStart + 0.5f - v0.pos.y );

		for ( int y = yStart; y < yEnd;
			  ++y,itEdge0 += dv0,itEdge1 += dv1 )
		{
			// Get start and end for x according to left rule
			const int xStart = (int)std::ceil( itEdge0.pos.x - 0.5f );
			const int xEnd = (int)std::ceil( itEdge1.pos.x - 0.5f );

			// Calculate the scan step for the texture
			const auto dtcLine = ( itEdge1.tc - itEdge0.tc ) / ( itEdge1.pos.x - itEdge0.pos.x );

			// Get texture coordinate and prestep
			auto itcLine = itEdge0.tc + dtcLine * ( (float)xStart + 0.5f - itEdge0.pos.x );

			for ( int x = xStart; x < xEnd;
				  ++x,itcLine += dtcLine )
			{
				gfx.PutPixel( x,y,effect.ps( itcLine ) );
			}
		}
	}

public:
	Effect effect;
private:
	Graphics& gfx;
	PubeScreenTransformer pst;
	Mat3 rotation = Mat3::Identity();
	Vec3 translation = { 0.0f,0.0f,0.0f };
};
