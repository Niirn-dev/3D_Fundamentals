#pragma once

#include "ChiliWin.h"
#include "Scene.h"
#include "Cube.h"
#include "IndexedTriangleList.h"
#include "Mat3.h"
#include "PubeScreenTransformer.h"
#include "Surface.h"
#include <memory>
#include <vector>
#include <range/v3/all.hpp>
#include "Pipeline.h"
#include "SolidColorEffect.h"

class SolidColorTwoCubeScene : public Scene
{
public:
	using Pipeline = Pipeline<SolidColorEffect>;
	using Vertex = Pipeline::Vertex;
public:
	SolidColorTwoCubeScene( Graphics& gfx,float cubeSize )
		:
		pipeline( gfx ),
		itlist0( Cube::MakeIndividualFaces<Vertex>( cubeSize ) ),
		itlist1( Cube::MakeIndividualFaces<Vertex>( cubeSize ) )
	{
		namespace rn = ranges;
		namespace vi = rn::views;

		for ( size_t i = 0; auto& [v0,v1] : vi::zip( itlist0.vertices,itlist1.vertices ) )
		{
			v0.tc = (Vec3)vColors[i / 4 % std::size( vColors )];
			v1.tc = (Vec3)vColors[i / 4 % std::size( vColors )];
			++i;
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
			Mat3::RotationX( -angleX ) *
			Mat3::RotationY( -angleY ) *
			Mat3::RotationZ( -angleZ ) );
		pipeline.BindTranslation( Vec3{ 0.0f,0.0f,3.0f } );
		pipeline.Draw( itlist0 );

		pipeline.BindRotation(
			Mat3::RotationX( angleX ) *
			Mat3::RotationY( angleY ) *
			Mat3::RotationZ( angleZ )
		);
		pipeline.BindTranslation( translation );
		pipeline.Draw( itlist1 );
	}
private:
	Pipeline pipeline;
	IndexedTriangleList<Pipeline::Vertex> itlist0;
	IndexedTriangleList<Pipeline::Vertex> itlist1;

	static constexpr float dTheta = PI / 2.0f;
	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	Vec3 translation = { 0.0f,0.0f,3.0f };

	static constexpr Color vColors[] = {
		Colors::Red,
		Colors::Blue,
		Colors::Green,
		Colors::Yellow,
		Colors::Cyan,
		Colors::Magenta
	};
};
