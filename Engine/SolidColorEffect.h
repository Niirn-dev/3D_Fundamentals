#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include <memory>
#include "Surface.h"
#include <string>
#include "Colors.h"
#include "DefaultVertexShader.h"
#include "Triangle.h"
#include <vector>

class SolidColorEffect
{

public:
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
			pos( pos )
		{}
		Vertex& operator=( const Vertex& rhs )
		{
			pos = rhs.pos;
			return *this;
		}
		Vertex& operator+=( const Vertex& rhs )
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex& operator-=( const Vertex& rhs )
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex	operator+( const Vertex& rhs ) const
		{
			return Vertex( *this ) += rhs;
		}
		Vertex	operator-( const Vertex& rhs ) const
		{
			return Vertex( *this ) -= rhs;
		}
		Vertex& operator*=( const float& rhs )
		{
			pos *= rhs;
			return *this;
		}
		Vertex	operator*( const float& rhs ) const
		{
			return Vertex( *this ) *= rhs;
		}
		Vertex& operator/=( const float& rhs )
		{
			pos /= rhs;
			return *this;
		}
		Vertex	operator/( const float& rhs ) const
		{
			return Vertex( *this ) /= rhs;
		}

	public:
		Vec3 pos;
	};

	using VertexShader = DefaultVertexShader<Vertex>;
	class GeometryShader
	{
	public:
		class GSVertex
		{
		public:
			GSVertex() = default;
			GSVertex( const Vec3& pos,const Vec3& color )
				:
				pos( pos ),
				color( color )
			{}
			GSVertex( const Vec3& pos,const GSVertex& src )
				:
				pos( pos ),
				color( src.color )
			{}
			GSVertex&	operator=( const GSVertex & rhs )
			{
				pos = rhs.pos;
				return *this;
			}
			GSVertex&	operator+=( const GSVertex & rhs )
			{
				pos += rhs.pos;
				return *this;
			}
			GSVertex&	operator-=( const GSVertex & rhs )
			{
				pos -= rhs.pos;
				return *this;
			}
			GSVertex	operator+( const GSVertex & rhs ) const
			{
				return GSVertex( *this ) += rhs;
			}
			GSVertex	operator-( const GSVertex & rhs ) const
			{
				return GSVertex( *this ) -= rhs;
			}
			GSVertex&	operator*=( const float& rhs )
			{
				pos *= rhs;
				return *this;
			}
			GSVertex	operator*( const float& rhs ) const
			{
				return GSVertex( *this ) *= rhs;
			}
			GSVertex&	operator/=( const float& rhs )
			{
				pos /= rhs;
				return *this;
			}
			GSVertex	operator/( const float& rhs ) const
			{
				return GSVertex( *this ) /= rhs;
			}

		public:
			Vec3 pos;
			Vec3 color;
		};
		using Output = typename GSVertex;
		using Input = VertexShader::Output;

		void BindLightDirection( const Vec3& d )
		{
			assert( d.LenSq() > 0.0f );
			dir = d.GetNormalized();
		}
		void BindDiffuseLight( const Vec3& diff )
		{
			diffuse = diff;
		}
		void BindAmbientLight( const Vec3& amb )
		{
			ambient = amb;
		}
		void BindColors( std::vector<Color> cs )
		{
			colors = std::move( cs );
		}

		Triangle<Output> operator()( const Input& v0,const Input& v1,const Input& v2,size_t id )
		{
			// get face color
			Vec3 c = (Vec3)colors[id / 2 % colors.size()];
			// apply dynamic lighting
			const Vec3 n = ( ( v1.pos - v0.pos ) % ( v2.pos - v1.pos ) ).GetNormalized();
			const Vec3 d = diffuse * std::max( 0.0f,-n * dir );
			c = c.GetHadamard( d + ambient ).Saturate();

			Output ov0 = { v0.pos,c };
			Output ov1 = { v1.pos,c };
			Output ov2 = { v2.pos,c };
			return { std::move( ov0 ),std::move( ov1 ),std::move( ov2 ) };
		}
	private:
		Vec3 dir = { 0.0f,0.0f,1.0f };
		Vec3 diffuse = { 1.0f,1.0f,1.0f };
		Vec3 ambient = { 0.08f,0.08f,0.08f };
		std::vector<Color> colors;
	};
	class PixelShader
	{
	public:
		template<typename Input>
		Color operator()( const Input& in )
		{
			return Color{ in.color };
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
