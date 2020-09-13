#pragma once

#include <memory>
#include <numeric>
#include "Vec3.h"

class ZBuffer
{
public:
	ZBuffer( int screenWidth,int screenHeight )
		:
		width( screenWidth ),
		height( screenHeight ),
		zBuffer( std::make_unique<float[]>( (size_t)width * height ) )
	{
	}
	float& At( int x,int y )
	{
		return zBuffer[(size_t)width * y + x];
	}
	const float& At( int x,int y ) const
	{
		return const_cast<ZBuffer*>( this )->At( x,y );
	}
	void Clear()
	{
		const int nDepth = width * height;
		for ( int i = 0; i < nDepth; ++i )
		{
			zBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}
	bool TestAndSet( int x,int y,float z )
	{
		if ( z < At( x,y ) )
		{
			At( x,y ) = z;
			return true;
		}
		return false;
	}

private:
	int width;
	int height;
	std::unique_ptr<float[]> zBuffer;
};
