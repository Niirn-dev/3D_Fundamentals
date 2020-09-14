#pragma once

#include "ChiliWin.h"
#include "Scene.h"
#include "Plane.h"
#include "IndexedTriangleList.h"
#include "Mat3.h"
#include "PubeScreenTransformer.h"
#include "Surface.h"
#include <memory>
#include "Pipeline.h"
#include "TextureWaveEffect.h"

class TexWavePlaneScene : public Scene
{
public:
	using Pipeline = Pipeline<TextureWaveEffect>;
	using Vertex = Pipeline::Vertex;
public:
	TexWavePlaneScene( Graphics& gfx,float planeWidth,float planeHeight,const std::wstring& texFilePath )
		:
		pipeline( gfx ),
		itlist( Plane::MakeSkinnedDouble<Vertex>( planeWidth,planeHeight,1.0f,20,20 ) )
	{
		pipeline.effect.ps.BindTexture( texFilePath );
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
		time += dt;
	}
	void Draw() override
	{
		pipeline.BeginFrame();

		pipeline.effect.vs.BindTime( time );
		pipeline.effect.vs.BindRotation(
			Mat3::RotationX( angleX ) *
			Mat3::RotationY( angleY ) *
			Mat3::RotationZ( angleZ ) );
		pipeline.effect.vs.BindTranslation( translation );

		pipeline.Draw( itlist );
	}
private:
	Pipeline pipeline;
	IndexedTriangleList<Pipeline::Vertex> itlist;

	float time = 0.0f;
	static constexpr float dTheta = PI / 2.0f;
	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	Vec3 translation = { 0.0f,0.0f,3.0f };
};
