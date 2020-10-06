#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat.h"
#include "Pipeline.h"
#include "SpecularPhongPointEffect.h"
#include "SolidEffect.h"
#include "Sphere.h"
#include "MouseTracker.h"
#include "Mouse.h"
#include "ChiliMath.h"

class SpecularPhongPointScene : public Scene
{
public:
	typedef Pipeline<SpecularPhongPointEffect> ModelPipeline;
	typedef Pipeline<SolidEffect> LightPipeline;
	typedef ModelPipeline::Vertex ModelVertex;
	typedef LightPipeline::Vertex LightVertex;
public:
	SpecularPhongPointScene( Mouse& mouse,Graphics& gfx,IndexedTriangleList<ModelVertex> tl )
		:
		pzb( std::make_shared<ZBuffer>( gfx.ScreenWidth,gfx.ScreenHeight ) ),
		mouse( mouse ),
		mt( std::make_unique<MouseTracker>() ),
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
			cameraPos += Vec4{ 0.0f,0.0f,1.0f,0.0f } * cameraRotationInv.GetTransposed() * cameraSpeed * dt;
		}
		if ( kbd.KeyIsPressed( 'A' ) )
		{
			cameraPos -= Vec4{ 1.0f,0.0f,0.0f,0.0f } *cameraRotationInv.GetTransposed() * cameraSpeed * dt;
		}
		if ( kbd.KeyIsPressed( 'S' ) )
		{
			cameraPos -= Vec4{ 0.0f,0.0f,1.0f,0.0f } *cameraRotationInv.GetTransposed() * cameraSpeed * dt;
		}
		if ( kbd.KeyIsPressed( 'D' ) )
		{
			cameraPos += Vec4{ 1.0f,0.0f,0.0f,0.0f } *cameraRotationInv.GetTransposed() * cameraSpeed * dt;
		}

		while ( !mouse.IsEmpty() )
		{
			const auto e = mouse.Read();
			switch ( e.GetType() )
			{
			case Mouse::Event::LPress:
				mt->Engage( e.GetPos() );
				break;
			case Mouse::Event::LRelease:
				mt->Disengage();
				break;
			case Mouse::Event::Move:
				if ( mt->IsEngaged() )
				{
					const auto delta = mt->Move( e.GetPos() );
					const auto dPitch = (float)delta.y * vtrack;
					const auto dYaw = (float)delta.x * htrack;
					pitch = std::clamp( pitch + dPitch,-PI / 2.0f,PI / 2.0f );
					yaw = std::clamp( yaw + dYaw,-PI,PI );
					cameraRotationInv = 
						Mat4::RotationY( yaw ) *
						Mat4::RotationX( pitch );
				}
				break;
			default:
				break;
			}
		}
	}
	virtual void Draw() override
	{
		const auto proj = Mat4::ProjectionHFOV( hfov,Graphics::AspectRatio,0.5f,4.0f );
		const auto camera = Mat4::Translation( -cameraPos ) * cameraRotationInv;
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
	Mouse& mouse;
	std::unique_ptr<MouseTracker> mt;

	IndexedTriangleList<ModelVertex> itlist;
	ModelPipeline pipeline;
	Vec3 modelPos = { 0.0f,0.0f,2.0f };

	static constexpr float hfov = 95.0f;
	static constexpr float vfov = hfov / Graphics::AspectRatio;

	static constexpr float htrack = to_radians( hfov ) / (float)Graphics::ScreenWidth;
	static constexpr float vtrack = to_radians( vfov ) / (float)Graphics::ScreenHeight;

	float pitch = 0.0f; // vertical (radians)
	float yaw = 0.0f; // horisontal (radians)

	Vec3 cameraPos = { 0.0f,0.0f,0.0f };
	Mat4 cameraRotationInv = Mat4::Identity();
	static constexpr float cameraSpeed = 1.0f;

	IndexedTriangleList<LightVertex> lightSphere;
	LightPipeline lightPipeline;
	Vec3 lightPos = { 0.0f,0.0f,0.5f };
	static constexpr float light_speed = 2.0f;
	static constexpr Color light_color = Colors::Yellow;
};