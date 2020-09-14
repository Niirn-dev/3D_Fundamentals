#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include <memory>
#include "Surface.h"
#include <string>
#include "Colors.h"
#include "DefaultVertexShader.h"

class SolidColorEffect
{

public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex( const Vec3& pos,const Vec3& tc )
			:
			pos( pos ),
			tc( tc )
		{}
		Vertex( const Vec3& pos,const Vertex& src )
			:
			pos( pos ),
			tc( src.tc )
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
		Vec3 tc; // normalized rgb values
	};

	using VertexShader = DefaultVertexShader<Vertex>;
	class PixelShader
	{
	public:
		template<typename Input>
		Color operator()( const Input& in )
		{
			return Color{ in };
		}
	};

public:
	VertexShader vs;
	PixelShader ps;
};
