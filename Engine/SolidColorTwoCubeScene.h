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
		itlist0( Cube::MakePlain<Vertex>( cubeSize ) ),
		itlist1( Cube::MakePlain<Vertex>( cubeSize ) )
	{
		std::vector<Color> colors = {
			Colors::Red,
			Colors::Blue,
			Colors::Green,
			Colors::Yellow,
			Colors::Cyan,
			Colors::Magenta
		};
		pipeline.effect.gs.BindColors( std::move( colors ) );
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

		if ( kbd.KeyIsPressed( 'T' ) )
		{
			lightDir *= Mat3::RotationX( dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'G' ) )
		{
			lightDir *= Mat3::RotationX( -dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'Y' ) )
		{
			lightDir *= Mat3::RotationY( dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'H' ) )
		{
			lightDir *= Mat3::RotationY( -dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'U' ) )
		{
			lightDir *= Mat3::RotationZ( dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'J' ) )
		{
			lightDir *= Mat3::RotationZ( -dTheta * dt );
		}

		if ( kbd.KeyIsPressed( 'I' ) )
		{
			lightDiffuse += Vec3{ 0.2f,0.2f,0.2f } *dt;
			lightDiffuse.Saturate();
		}
		if ( kbd.KeyIsPressed( 'K' ) )
		{
			lightDiffuse -= Vec3{ 0.2f,0.2f,0.2f } *dt;
			lightDiffuse.Saturate();
		}
		if ( kbd.KeyIsPressed( 'O' ) )
		{
			lightAmbient += Vec3{ 0.1f,0.1f,0.1f } *dt;
			lightAmbient.Saturate();
		}
		if ( kbd.KeyIsPressed( 'L' ) )
		{
			lightAmbient -= Vec3{ 0.1f,0.1f,0.1f } *dt;
			lightAmbient.Saturate();
		}
	}
	void Draw() override
	{
		pipeline.BeginFrame();

		pipeline.effect.gs.BindLightDirection( lightDir );
		pipeline.effect.gs.BindDiffuseLight( lightDiffuse );
		pipeline.effect.gs.BindAmbientLight( lightAmbient );

		pipeline.effect.vs.BindRotation(
			Mat3::RotationX( -angleX ) *
			Mat3::RotationY( -angleY ) *
			Mat3::RotationZ( -angleZ ) );
		pipeline.effect.vs.BindTranslation( Vec3{ 0.0f,0.0f,3.0f } );
		pipeline.Draw( itlist0 );

		pipeline.effect.vs.BindRotation(
			Mat3::RotationX( angleX ) *
			Mat3::RotationY( angleY ) *
			Mat3::RotationZ( angleZ )
		);
		pipeline.effect.vs.BindTranslation( translation );
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

	Vec3 lightDir = { 0.0f,0.0f,1.0f };
	Vec3 lightDiffuse = { 1.0f,1.0f,1.0f };
	Vec3 lightAmbient = { 0.08f,0.08f,0.08f };
};
