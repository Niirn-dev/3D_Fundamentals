#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include <memory>
#include "Surface.h"
#include <string>
#include "Colors.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"

class BlendColorEffect
{
public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex( const Vec3& pos,const Vec3& color )
			:
			pos( pos ),
			color( color )
		{}
		Vertex( const Vec3& pos,const Vertex& src )
			:
			pos( pos ),
			color( src.color )
		{}
		Vertex& operator=( const Vertex& rhs )
		{
			pos = rhs.pos;
			color = rhs.color;
			return *this;
		}
		Vertex& operator+=( const Vertex& rhs )
		{
			pos += rhs.pos;
			color += rhs.color;
			return *this;
		}
		Vertex& operator-=( const Vertex& rhs )
		{
			pos -= rhs.pos;
			color -= rhs.color;
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
			color *= rhs;
			return *this;
		}
		Vertex	operator*( const float& rhs ) const
		{
			return Vertex( *this ) *= rhs;
		}
		Vertex& operator/=( const float& rhs )
		{
			pos /= rhs;
			color /= rhs;
			return *this;
		}
		Vertex	operator/( const float& rhs ) const
		{
			return Vertex( *this ) /= rhs;
		}

	public:
		Vec3 pos;
		Vec3 color; // normalized rgb values
	};

	using VertexShader = DefaultVertexShader<Vertex>;
	using GeometryShader = DefaultGeometryShader<Vertex>;
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
