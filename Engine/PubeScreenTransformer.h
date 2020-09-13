#pragma once

#include "Graphics.h"
#include "Vec3.h"

class PubeScreenTransformer
{
public:
	PubeScreenTransformer()
		:
		xFactor( (float)Graphics::ScreenWidth / 2.0f ),
		yFactor( (float)Graphics::ScreenHeight / 2.0f )
	{
	}

	template<class Vertex>
	Vertex& Transform( Vertex& v )
	{
		// calculate the inverse of z coordinate
		const float zInv = 1.0f / v.pos.z;

		// apply perspective divide for the entire vertex
		v *= zInv;

		// transform vertex position from view space to screen space
		v.pos.x = ( v.pos.x + 1.0f ) * xFactor;
		v.pos.y = ( -v.pos.y + 1.0f ) * yFactor;

		// store the inverse of z in the z coordinate of the vertex 
		// to be able to restore the interpolated z coordinate later
		v.pos.z = zInv;

		return v;
	}
	template<class Vertex>
	Vertex GetTransformed( const Vertex& v )
	{
		return Transform( Vertex( v ) );
	}

private:
	const float xFactor;
	const float yFactor;
};
