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

	Vec3& Transform( Vec3& v )
	{
		const float zInv = 1.0f / v.z;
		v.x = ( v.x * zInv + 1.0f ) * xFactor;
		v.y = ( -v.y * zInv + 1.0f ) * yFactor;
		return v;
	}
	Vec3 GetTransformed( const Vec3& v )
	{
		return Transform( Vec3( v ) );
	}

private:
	const float xFactor;
	const float yFactor;
};
