#pragma once

#include "Pipeline.h"
#include "DefaultGeometryShader.h"

class PhongPointEffect
{
public:
	// the vertex type that will be input into the pipeline
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex( const Vec3& pos )
			:
			pos( pos )
		{}
		Vertex( const Vec3& pos,const Vertex& src )
			:
			n( src.n ),
			pos( pos )
		{}
		Vertex( const Vec3& pos,const Vec3& n )
			:
			n( n ),
			pos( pos )
		{}
		Vertex& operator+=( const Vertex& rhs )
		{
			pos += rhs.pos;
			n += rhs.n;
			return *this;
		}
		Vertex operator+( const Vertex& rhs ) const
		{
			return Vertex( *this ) += rhs;
		}
		Vertex& operator-=( const Vertex& rhs )
		{
			pos -= rhs.pos;
			n -= rhs.n;
			return *this;
		}
		Vertex operator-( const Vertex& rhs ) const
		{
			return Vertex( *this ) -= rhs;
		}
		Vertex& operator*=( float rhs )
		{
			pos *= rhs;
			n *= rhs;
			return *this;
		}
		Vertex operator*( float rhs ) const
		{
			return Vertex( *this ) *= rhs;
		}
		Vertex& operator/=( float rhs )
		{
			pos /= rhs;
			n /= rhs;
			return *this;
		}
		Vertex operator/( float rhs ) const
		{
			return Vertex( *this ) /= rhs;
		}
	public:
		Vec3 pos;
		Vec3 n;
	};
	// calculate color based on normal to light angle
	// interpolate color and normal information for per-pixel shading later
	class VertexShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output( const Vec3& pos )
				:
				pos( pos )
			{}
			Output( const Vec3& pos,const Output& src )
				:
				world_pos( src.world_pos ),
				n( src.n ),
				color( src.color ),
				pos( pos )
			{}
			Output( const Vec3& world_pos,const Vec3& pos,const Vec3& n,const Vec3& color )
				:
				world_pos( world_pos ),
				color( color ),
				n( n ),
				pos( pos )
			{}
			Output& operator+=( const Output& rhs )
			{
				world_pos += rhs.world_pos;
				pos += rhs.pos;
				n += rhs.n;
				color += rhs.color;
				return *this;
			}
			Output operator+( const Output& rhs ) const
			{
				return Output( *this ) += rhs;
			}
			Output& operator-=( const Output& rhs )
			{
				world_pos -= rhs.world_pos;
				pos -= rhs.pos;
				n -= rhs.n;
				color -= rhs.color;
				return *this;
			}
			Output operator-( const Output& rhs ) const
			{
				return Output( *this ) -= rhs;
			}
			Output& operator*=( float rhs )
			{
				world_pos *= rhs;
				pos *= rhs;
				n *= rhs;
				color *= rhs;
				return *this;
			}
			Output operator*( float rhs ) const
			{
				return Output( *this ) *= rhs;
			}
			Output& operator/=( float rhs )
			{
				world_pos /= rhs;
				pos /= rhs;
				n /= rhs;
				color /= rhs;
				return *this;
			}
			Output operator/( float rhs ) const
			{
				return Output( *this ) /= rhs;
			}
		public:
			Vec3 world_pos;
			Vec3 pos;
			Vec3 n;
			Vec3 color;
		};
	public:
		void BindRotation( const Mat3& rotation_in )
		{
			rotation = rotation_in;
		}
		void BindTranslation( const Vec3& translation_in )
		{
			translation = translation_in;
		}
		Output operator()( const Vertex& v ) const
		{
			// get new vertex position (since light sorce is static and is not rotated/translated)
			const auto new_v_pos = v.pos * rotation + translation;
			const auto new_v_n = v.n * rotation;
			return{ new_v_pos,new_v_pos,new_v_n,color };
		}
		void SetMaterialColor( Color c )
		{
			color = Vec3( c );
		}
	private:
		Mat3 rotation = Mat3::Identity();
		Vec3 translation = { 0.0f,0.0f,0.0f };
		Vec3 color = { 0.8f,0.85f,1.0f };
	};
	// default gs passes vertices through and outputs triangle
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
	// invoked for each pixel of a triangle
	// takes an input of attributes that are the
	// result of interpolating vertex attributes
	// and outputs a color based on the distance and direction from pixel to light source
	class PixelShader
	{
	public:
		template<class Input = GeometryShader::Output>
		Color operator()( const Input& in ) const
		{
			// get dir to light source
			const auto v_to_l = light_position - in.world_pos;
			const auto light_distance = v_to_l.Len();
			const auto light_dir = light_distance == 0.0f ? Vec3{ 0.0f,0.0f,0.0f } : v_to_l / light_distance;

			// calculate intensity based on angle of incidence
			const auto d = light_diffuse * std::max( 0.0f,in.n.GetNormalized() * light_dir );
			// adjust intensity based on distance to light source
			const auto i = d / ( light_quadratic * sq( light_distance ) + light_linear * light_distance + light_constant );
			// add diffuse+ambient, filter by material color, saturate and scale
			const auto c = in.color.GetHadamard( i + light_ambient ).Saturate() * 255.0f;

			return Color( c );
		}
		void SetDiffuseLight( const Vec3& c )
		{
			light_diffuse = { c.x,c.y,c.z };
		}
		void SetAmbientLight( const Vec3& c )
		{
			light_ambient = { c.x,c.y,c.z };
		}
		void SetLightPosition( const Vec3& pos )
		{
			light_position = pos;
		}

	private:
		Vec3 light_position = { 0.0f,0.0f,1.0f };
		Vec3 light_diffuse = { 1.0f,1.0f,1.0f };
		Vec3 light_ambient = { 0.1f,0.1f,0.1f };

		// coefficients for light intensity equation
		const float light_quadratic = 1.2f;
		const float light_linear = 2.1f;
		const float light_constant = 1.0f;
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};