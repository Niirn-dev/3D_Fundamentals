#pragma once

#include "ChiliWin.h"
#include "Scene.h"
#include "Cube.h"
#include "IndexedTriangleList.h"
#include "Mat3.h"
#include "PubeScreenTransformer.h"
#include "Surface.h"
#include <memory>
#include "Pipeline.h"
#include "BlendColorEffect.h"

class BlendColorCubeScene : public Scene
{
public:
	using Pipeline = Pipeline<BlendColorEffect>;
	using Vertex = Pipeline::Vertex;
public:
	BlendColorCubeScene( Graphics& gfx,float cubeSize )
		:
		pipeline( gfx ),
		itlist( Cube::MakePlain<Vertex>( cubeSize ) )
	{
		for ( size_t i = 0; auto& v : itlist.vertices )
		{
			v.tc = (Vec3)vColors[i++ % std::size( vColors )];
		}
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
			translation = Vec3{ 0.0f,0.0f,translation.z + 2.0f * dt };
		}
		if ( kbd.KeyIsPressed( 'F' ) )
		{
			translation = Vec3{ 0.0f,0.0f,std::max( 2.0f,translation.z - 2.0f * dt ) };
		}
	}
	void Draw() override
	{
		pipeline.BindRotation(
			Mat3::RotationX( angleX ) *
			Mat3::RotationY( angleY ) *
			Mat3::RotationZ( angleZ ) );
		pipeline.BindTranslation( translation );

		pipeline.Draw( itlist );
	}
private:
	Pipeline pipeline;
	IndexedTriangleList<Pipeline::Vertex> itlist;

	static constexpr float dTheta = PI / 2.0f;
	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	Vec3 translation = { 0.0f,0.0f,3.0f };

	static constexpr Color vColors[] = {
		Colors::White,
		Colors::Red,
		Colors::Blue,
		Colors::Green,
		Colors::Black,
		Colors::Yellow,
		Colors::Cyan,
		Colors::Magenta
	};
};
