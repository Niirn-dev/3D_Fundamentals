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
#include "ZBuffer.h"

template<class Effect>
class Pipeline
{
public:
	using Vertex = typename Effect::Vertex;
	using VSOut = typename Effect::VertexShader::Output;
	using GSOut = typename Effect::GeometryShader::Output;
public:
	Pipeline( Graphics& gfx )
		:
		gfx( gfx ),
		zbuff( Graphics::ScreenWidth,Graphics::ScreenHeight )
	{
	}

	void BeginFrame()
	{
		zbuff.Clear();
	}
	void Draw( const IndexedTriangleList<Vertex>& list )
	{
		ProcessVertices( list.vertices,list.indices );
	}

private:
	// Vertex Transformer
	// transforms vertices and then passes vtx & idx lists to triangle assembler
	void ProcessVertices( const std::vector<Vertex>& verts,const std::vector<size_t>& indices )
	{
		// create vertex vector for output
		std::vector<VSOut> verticesOut;
		verticesOut.reserve( verts.size() );

		// transform vertices using rotation matris and translation vector
		std::transform( verts.begin(),verts.end(),
						std::back_inserter( verticesOut ),
						effect.vs);

		// assembe triangles from the stream of indices and vertices
		AssembleTriangles( verticesOut,indices );
	}

	// Triangle Assembler
	// assembles indexed vertex stream into triangles and passes them to post process
	// culls (does not send) back facing triangles
	void AssembleTriangles( const std::vector<VSOut>& verts,const std::vector<size_t>& indices )
	{
		// assemble triangles in the stream and process
		for ( size_t i = 0ull; i < indices.size() / 3; ++i )
		{
			const VSOut& v0 = verts[indices[i * 3]];
			const VSOut& v1 = verts[indices[i * 3 + 1]];
			const VSOut& v2 = verts[indices[i * 3 + 2]];

			// cull backfacing triangles with the help of cross product (%)
			if ( ( v1.pos - v0.pos ) % ( v2.pos - v0.pos ) * v0.pos < 0.0f )
			{
				// process 3 vertices of a frontfacing triangle into a triangle object
				ProcessTriangle( v0,v1,v2 );
			}
		}
	}
	// triangle processing function
	// takes 3 vertices to generate triangle
	// sends generated triangle to post-processing
	void ProcessTriangle( const VSOut& v0,const VSOut& v1,const VSOut& v2 )
	{
		// generate triangle from 3 vertices (geometry shader will go here later)
		// and send to post-processing
		ProcessTriangleVertices( effect.gs( v0,v1,v2 ) );
	}

	// Perspective/Screen Transformer
	// perform perspective and viewport transformations
	void ProcessTriangleVertices( Triangle<GSOut>& triangle )
	{
		// apply perspective divide and screen transform to all 3 verices
		pst.Transform( triangle.v0 );
		pst.Transform( triangle.v1 );
		pst.Transform( triangle.v2 );

		// draw the triangle
		DrawTriangle( triangle );
	}

	// Triangle Rasterizer
	void DrawTriangle( const Triangle<GSOut>& triangle )
	{
		// Get pointers to vertices for easier swapping
		const auto* pv0 = &triangle.v0;
		const auto* pv1 = &triangle.v1;
		const auto* pv2 = &triangle.v2;

		// Sort vertices by y
		if ( pv0->pos.y > pv1->pos.y ) std::swap( pv0,pv1 );
		if ( pv0->pos.y > pv2->pos.y ) std::swap( pv0,pv2 );
		if ( pv1->pos.y > pv2->pos.y ) std::swap( pv1,pv2 );

		// Establish the type of the passed in triangle
		if ( pv0->pos.y == pv1->pos.y ) // Flat top triangle
		{
			// Sort vertices by x
			if ( pv0->pos.x > pv1->pos.x ) std::swap( pv0,pv1 );
			DrawTriangleFlatTop( *pv0,*pv1,*pv2 );
		}
		else if ( pv1->pos.y == pv2->pos.y ) // Flat bottom triangle
		{
			// Sort vertices by x
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
	void DrawTriangleFlatTop( const GSOut& v0,
							  const GSOut& v1,
							  const GSOut& v2 )
	{
		// Get dVertex / dy
		const float dy = v2.pos.y - v0.pos.y;
		const auto dv0 = ( v2 - v0 ) / dy;
		const auto dv1 = ( v2 - v1 ) / dy;

		// Get interpolated edges
		auto itEdge1 = v1;

		DrawTriangleFlat( v0,v1,v2,dv0,dv1,itEdge1 );
	}
	void DrawTriangleFlatBottom( const GSOut& v0,
								 const GSOut& v1,
								 const GSOut& v2 )
	{
		// Get dVertex / dy
		const float dy = v2.pos.y - v0.pos.y;
		const auto dv0 = ( v1 - v0 ) / dy;
		const auto dv1 = ( v2 - v0 ) / dy;

		// Get the interpolated edges
		auto itEdge1 = v0;

		DrawTriangleFlat( v0,v1,v2,dv0,dv1,itEdge1 );
	}
	void DrawTriangleFlat( const GSOut& v0,
						   const GSOut& v1,
						   const GSOut& v2,
						   const GSOut& dv0,
						   const GSOut& dv1,
						   GSOut& itEdge1 )
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
			// get start and end for x according to left rule
			const int xStart = (int)std::ceil( itEdge0.pos.x - 0.5f );
			const int xEnd = (int)std::ceil( itEdge1.pos.x - 0.5f );

			// get the starting point for scanline interpolant
			auto itcLine = itEdge0;

			// calculate delta scan line interpolant / dx
			const auto dx = itEdge1.pos.x - itEdge0.pos.x;
			const auto dtcLine = ( itEdge1 - itcLine ) / dx;

			// prestep scanline interpolant
			itcLine += dtcLine * ( (float)xStart + 0.5f - itEdge0.pos.x );

			for ( int x = xStart; x < xEnd;
				  ++x,itcLine += dtcLine )
			{
				// recover interpolated z from interpolated 1/z
				const float z = 1.0f / itcLine.pos.z;

				// check if triangle pixel is the closest one to the camera yet
				if ( zbuff.TestAndSet( x,y,z ) )
				{
					// recover interpolated attributes
					const auto attr = itcLine * z;
					// invoke pixel shader with interpolated vertex attributes
					// and use result to set the pixel color on the screen
					gfx.PutPixel( x,y,effect.ps( attr.tc ) );
				}
			}
		}
	}

public:
	Effect effect;
private:
	Graphics& gfx;
	ZBuffer zbuff;
	PubeScreenTransformer pst;
};
