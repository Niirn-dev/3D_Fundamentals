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
		light_sphere( std::move( Sphere::GetPlain<LightVertex>( 0.05f ) ) ),
		lightPipeline( gfx,pzb ),
		Scene( "gouraud shader scene free mesh" )
	{
		itlist.AdjustToTrueCenter();
		offset_z = itlist.GetRadius() * 1.6f;

		for ( auto& v : light_sphere.vertices )
		{
			v.color = light_color;
		}
	}
	virtual void Update( Keyboard& kbd,Mouse& mouse,float dt ) override
	{
		if ( kbd.KeyIsPressed( 'Q' ) )
		{
			theta_x = wrap_angle( theta_x + dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'W' ) )
		{
			theta_y = wrap_angle( theta_y + dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'E' ) )
		{
			theta_z = wrap_angle( theta_z + dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'A' ) )
		{
			theta_x = wrap_angle( theta_x - dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'S' ) )
		{
			theta_y = wrap_angle( theta_y - dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'D' ) )
		{
			theta_z = wrap_angle( theta_z - dTheta * dt );
		}
		if ( kbd.KeyIsPressed( 'U' ) )
		{
			light_pos.x += light_speed * dt;
		}
		if ( kbd.KeyIsPressed( 'I' ) )
		{
			light_pos.y += light_speed * dt;
		}
		if ( kbd.KeyIsPressed( 'O' ) )
		{
			light_pos.z += light_speed * dt;
		}
		if ( kbd.KeyIsPressed( 'J' ) )
		{
			light_pos.x -= light_speed * dt;
		}
		if ( kbd.KeyIsPressed( 'K' ) )
		{
			light_pos.y -= light_speed * dt;
		}
		if ( kbd.KeyIsPressed( 'L' ) )
		{
			light_pos.z -= light_speed * dt;
		}
		if ( kbd.KeyIsPressed( 'R' ) )
		{
			offset_z += 0.2f * dt;
		}
		if ( kbd.KeyIsPressed( 'F' ) )
		{
			offset_z -= 0.2f * dt;
		}
	}
	virtual void Draw() override
	{
		pipeline.BeginFrame();
		// set pipeline transform
		pipeline.effect.vs.BindWorld( 
			Mat4::RotationX( theta_x ) *
			Mat4::RotationY( theta_y ) *
			Mat4::RotationZ( theta_z ) *
			Mat4::Translation( 0.0f,0.0f,offset_z )
		);
		pipeline.effect.vs.BindProjection(
			Mat4::Projection( 2.0f,2.0f,1.0f,10.0f )
		);
		pipeline.effect.ps.SetLightPosition( light_pos );
		// render triangles
		pipeline.Draw( itlist );

		lightPipeline.effect.vs.BindWorld(
			Mat4::Identity() *
			Mat4::Translation( 
				light_pos.x,
				light_pos.y,
				light_pos.z
			)
		);
		lightPipeline.effect.vs.BindProjection(
			Mat4::Projection( 2.0f,2.0f,1.0f,10.0f )
		);

		lightPipeline.Draw( light_sphere );
	}
private:
	std::shared_ptr<ZBuffer> pzb;

	IndexedTriangleList<ModelVertex> itlist;
	ModelPipeline pipeline;
	static constexpr float dTheta = PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	IndexedTriangleList<LightVertex> light_sphere;
	LightPipeline lightPipeline;
	Vec3 light_pos = { 0.0f,0.0f,0.5f };
	static constexpr float light_speed = 2.0f;
	static constexpr Color light_color = Colors::Yellow;
};