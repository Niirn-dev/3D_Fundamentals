#pragma once

#include "ChiliWin.h"
#include "Scene.h"
#include "Cube.h"
#include "IndexedTriangleList.h"
#include "Mat3.h"
#include "PubeScreenTransformer.h"
#include "Surface.h"
#include "TexVertex.h"

class TexCubeScene : public Scene
{
public:
	TexCubeScene( PubeScreenTransformer& pst,float cubeSize,const Surface& tex )
		:
		pst( pst ),
		c( cubeSize ),
		tex( tex )
	{
	}

	void Update( Keyboard& kbd,Mouse& mouse,float dt ) override
	{
		if ( kbd.KeyIsPressed( 'Q' ) )
		{
			angleX = wrap_angle( angleX + dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'A' ) )
		{
			angleX = wrap_angle( angleX - dTheta * dt );
		}

		if ( kbd.KeyIsPressed( 'W' ) )
		{
			angleY = wrap_angle( angleY + dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'S' ) )
		{
			angleY = wrap_angle( angleY - dTheta * dt );
		}

		if ( kbd.KeyIsPressed( 'E' ) )
		{
			angleZ = wrap_angle( angleZ + dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'D' ) )
		{
			angleZ = wrap_angle( angleZ - dTheta * dt );
		}

		if ( kbd.KeyIsPressed( 'R' ) )
		{
			offsetZ = offsetZ + 2.0f * dt;
		}
		if ( kbd.KeyIsPressed( 'F' ) )
		{
			offsetZ = std::max( 2.0f,offsetZ - 2.0f * dt );;
		}
	}
	void Draw( Graphics& gfx ) override
	{
		auto list = c.GetTriangleTexList();

		auto rot =
			Mat3::RotationX( angleX ) *
			Mat3::RotationY( angleY ) *
			Mat3::RotationZ( angleZ );
		for ( auto& tv : list.vertices )
		{
			tv.pos *= rot;
			tv.pos.z += offsetZ;
		}

		for ( size_t i = 0; i < list.indices.size() / 3; ++i )
		{
			const TexVertex& v0 = list.vertices[list.indices[3 * i]];
			const TexVertex& v1 = list.vertices[list.indices[3 * i + 1]];
			const TexVertex& v2 = list.vertices[list.indices[3 * i + 2]];

			if ( ( v1.pos - v0.pos ) % ( v2.pos - v0.pos ) * v0.pos >= 0.0f )
			{
				list.cullingMask[i] = false;
			}
		}

		for ( auto& tv : list.vertices )
		{
			pst.Transform( tv.pos );
		}

		for ( size_t i = 0; i < list.indices.size() / 3; ++i )
		{
			if ( list.cullingMask[i] )
			{
				gfx.DrawTriangleTex(
					list.vertices[list.indices[3 * i]],
					list.vertices[list.indices[3 * i + 1]],
					list.vertices[list.indices[3 * i + 2]],
					tex );
			}
		}
	}
private:
	PubeScreenTransformer& pst;
	Cube c;
	const Surface& tex;
	static constexpr float dTheta = PI / 4.0f;
	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	float offsetZ = 3.0f;
};
