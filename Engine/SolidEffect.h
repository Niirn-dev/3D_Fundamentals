#pragma once

#include "Pipeline.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"

// solid color attribute not interpolated
class SolidEffect
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
			color( src.color ),
			pos( pos )
		{}
		Vertex( const Vec3& pos,const Color& color )
			:
			color( color ),
			pos( pos )
		{}
		Vertex& operator+=( const Vertex& rhs )
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex operator+( const Vertex& rhs ) const
		{
			return Vertex( *this ) += rhs;
		}
		Vertex& operator-=( const Vertex& rhs )
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex operator-( const Vertex& rhs ) const
		{
			return Vertex( *this ) -= rhs;
		}
		Vertex& operator*=( float rhs )
		{
			pos *= rhs;
			return *this;
		}
		Vertex operator*( float rhs ) const
		{
			return Vertex( *this ) *= rhs;
		}
		Vertex& operator/=( float rhs )
		{
			pos /= rhs;
			return *this;
		}
		Vertex operator/( float rhs ) const
		{
			return Vertex( *this ) /= rhs;
		}
	public:
		Vec3 pos;
		Color color;
	};
	// vs applies world and projection transformation
	// moves object from object space to perspective space
	class VertexShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output( const Vec4& pos )
				:
				pos( pos )
			{}
			Output( const Vec4& pos,const Output& src )
				:
				color( src.color ),
				pos( pos )
			{}
			Output( const Vec4& pos,const Color& color )
				:
				color( color ),
				pos( pos )
			{}
			Output& operator+=( const Output& rhs )
			{
				pos += rhs.pos;
				return *this;
			}
			Output operator+( const Output& rhs ) const
			{
				return Output( *this ) += rhs;
			}
			Output& operator-=( const Output& rhs )
			{
				pos -= rhs.pos;
				return *this;
			}
			Output operator-( const Output& rhs ) const
			{
				return Output( *this ) -= rhs;
			}
			Output& operator*=( float rhs )
			{
				pos *= rhs;
				return *this;
			}
			Output operator*( float rhs ) const
			{
				return Output( *this ) *= rhs;
			}
			Output& operator/=( float rhs )
			{
				pos /= rhs;
				return *this;
			}
			Output operator/( float rhs ) const
			{
				return Output( *this ) /= rhs;
			}
		public:
			Vec4 pos;
			Color color;
		};

		void BindWorld( const Mat4& transformation_in )
		{
			world = transformation_in;
		}
		void BindProjection( const Mat4& transformation_in )
		{
			proj = transformation_in;
		}
		Output operator()( const Vertex& in )
		{
			const auto worldProj = world * proj;
			return { Vec4( in.pos ) * worldProj,in.color };
		}

	private:
		Mat4 world = Mat4::Identity();
		Mat4 proj  = Mat4::Identity();
	};
	// default gs passes vertices through and outputs triangle
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
	// invoked for each pixel of a triangle
	// takes an input of attributes that are the
	// result of interpolating vertex attributes
	// and outputs a color
	class PixelShader
	{
	public:
		template<class I>
		Color operator()( const I& in ) const
		{
			return in.color;
		}
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};