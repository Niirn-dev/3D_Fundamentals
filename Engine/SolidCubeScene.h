#pragma once

#include "ChiliWin.h"
#include "Scene.h"
#include "Cube.h"
#include "IndexedTriangleList.h"
#include "Mat3.h"
#include "PubeScreenTransformer.h"

class SolidCubeScene : public Scene
{
public:
	SolidCubeScene( PubeScreenTransformer& pst,float cubeSize )
		:
		pst( pst ),
		c( cubeSize )
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
		auto list = c.GetTriangleList();

		auto rot =
			Mat3::RotationX( angleX ) *
			Mat3::RotationY( angleY ) *
			Mat3::RotationZ( angleZ );
		for ( auto& v : list.vertices )
		{
			v *= rot;
			v.z += offsetZ;
		}

		for ( size_t i = 0; i < list.indices.size() / 3; ++i )
		{
			const Vec3& v0 = list.vertices[list.indices[3 * i]];
			const Vec3& v1 = list.vertices[list.indices[3 * i + 1]];
			const Vec3& v2 = list.vertices[list.indices[3 * i + 2]];

			if ( ( v1 - v0 ) % ( v2 - v0 ) * v0 >= 0.0f )
			{
				list.cullingMask[i] = false;
			}
		}

		for ( auto& v : list.vertices )
		{
			pst.Transform( v );
		}

		const Color c[] = {
			Colors::White,
			Colors::Red,
			Colors::Cyan,
			Colors::Gray,
			Colors::Green,
			Colors::Blue,
			Colors::Yellow
		};

		for ( size_t i = 0; i < list.indices.size() / 3; ++i )
		{
			if ( list.cullingMask[i] )
			{
				gfx.DrawTriangle(
					list.vertices[list.indices[3 * i]],
					list.vertices[list.indices[3 * i + 1]],
					list.vertices[list.indices[3 * i + 2]],
					c[i % std::size( c )] );
			}
		}
	}
private:
	PubeScreenTransformer& pst;
	Cube c;
	static constexpr float dTheta = PI / 4.0f;
	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	float offsetZ = 3.0f;
};
