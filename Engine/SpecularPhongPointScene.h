#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat.h"
#include "Pipeline.h"
#include "SpecularPhongPointEffect.h"
#include "SolidEffect.h"
#include "Sphere.h"

class SpecularPhongPointScene : public Scene
{
public:
	typedef Pipeline<SpecularPhongPointEffect> ModelPipeline;
	typedef Pipeline<SolidEffect> LightPipeline;
	typedef ModelPipeline::Vertex ModelVertex;
	typedef LightPipeline::Vertex LightVertex;
public:
	SpecularPhongPointScene( Graphics& gfx,IndexedTriangleList<ModelVertex> tl )
		:
		pzb( std::make_shared<ZBuffer>( gfx.ScreenWidth,gfx.ScreenHeight ) ),
		itlist( std::move( tl ) ),
		pipeline( gfx,pzb ),
		lightSphere( std::move( Sphere::GetPlain<LightVertex>( 0.05f ) ) ),
		lightPipeline( gfx,pzb ),
		Scene( "gouraud shader scene free mesh" )
	{
		itlist.AdjustToTrueCenter();
		modelPos.z = itlist.GetRadius() * 1.6f;

		for ( auto& v : lightSphere.vertices )
		{
			v.color = light_color;
		}
	}
	virtual void Update( Keyboard& kbd,Mouse& mouse,float dt ) override
	{
		if ( kbd.KeyIsPressed( 'W' ) )
		{
			cameraPos.z += cameraSpeed * dt;
		}
		if ( kbd.KeyIsPressed( 'A' ) )
		{
			cameraPos.x -= cameraSpeed * dt;
		}
		if ( kbd.KeyIsPressed( 'S' ) )
		{
			cameraPos.z -= cameraSpeed * dt;
		}
		if ( kbd.KeyIsPressed( 'D' ) )
		{
			cameraPos.x += cameraSpeed * dt;
		}
	}
	virtual void Draw() override
	{
		const auto proj = Mat4::ProjectionHFOV( 100.0f,Graphics::AspectRatio,0.5f,4.0f );
		const auto camera = Mat4::Translation( -cameraPos );
		pipeline.BeginFrame();
		// set pipeline transform
		pipeline.effect.vs.BindWorld(
			Mat4::Translation( modelPos )
		);
		pipeline.effect.vs.BindCamera( camera );
		pipeline.effect.vs.BindProjection( proj );
		pipeline.effect.ps.SetLightPosition( Vec4( lightPos ) * camera );
		// render triangles
		pipeline.Draw( itlist );

		lightPipeline.effect.vs.BindWorld(
			Mat4::Identity() *
			Mat4::Translation( 
				lightPos
			)
		);
		lightPipeline.effect.vs.BindCamera( camera );
		lightPipeline.effect.vs.BindProjection( proj );

		lightPipeline.Draw( lightSphere );
	}
private:
	std::shared_ptr<ZBuffer> pzb;

	IndexedTriangleList<ModelVertex> itlist;
	ModelPipeline pipeline;
	Vec3 modelPos = { 0.0f,0.0f,2.0f };

	Vec3 cameraPos = { 0.0f,0.0f,0.0f };
	static constexpr float cameraSpeed = 2.0f;

	IndexedTriangleList<LightVertex> lightSphere;
	LightPipeline lightPipeline;
	Vec3 lightPos = { 0.0f,0.0f,0.5f };
	static constexpr float light_speed = 2.0f;
	static constexpr Color light_color = Colors::Yellow;
};