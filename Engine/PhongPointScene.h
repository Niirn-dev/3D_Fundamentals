#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "PhongPointEffect.h"
#include "SolidEffect.h"
#include "Sphere.h"

class PhongPointScene : public Scene
{
public:
	typedef Pipeline<PhongPointEffect> ModelPipeline;
	typedef Pipeline<SolidEffect> LightPipeline;
	typedef ModelPipeline::Vertex ModelVertex;
	typedef LightPipeline::Vertex LightVertex;
public:
	PhongPointScene( Graphics& gfx,IndexedTriangleList<ModelVertex> tl )
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
		// generate rotation matrix from euler angles
		// translation from offset
		const Mat3 rot =
			Mat3::RotationX( theta_x ) *
			Mat3::RotationY( theta_y ) *
			Mat3::RotationZ( theta_z );
		const Vec3 trans = { 0.0f,0.0f,offset_z };
		// set pipeline transform
		pipeline.effect.vs.BindRotation( rot );
		pipeline.effect.vs.BindTranslation( trans );
		pipeline.effect.ps.SetLightPosition( light_pos );
		// render triangles
		pipeline.Draw( itlist );

		lightPipeline.effect.vs.BindRotation( Mat3::Identity() );
		lightPipeline.effect.vs.BindTranslation( light_pos );

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