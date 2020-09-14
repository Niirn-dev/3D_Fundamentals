#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include <memory>
#include "Surface.h"
#include <string>
#include "Colors.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"

class TextureEffectClamp
{

public:
	class Vertex
	{
	public:
		Vertex( const Vec3& pos,const Vec2& tc )
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
			tc = rhs.tc;
			return *this;
		}
		Vertex& operator+=( const Vertex& rhs )
		{
			pos += rhs.pos;
			tc += rhs.tc;
			return *this;
		}
		Vertex& operator-=( const Vertex& rhs )
		{
			pos -= rhs.pos;
			tc -= rhs.tc;
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
			tc *= rhs;
			return *this;
		}
		Vertex	operator*( const float& rhs ) const
		{
			return Vertex( *this ) *= rhs;
		}
		Vertex& operator/=( const float& rhs )
		{
			pos /= rhs;
			tc /= rhs;
			return *this;
		}
		Vertex	operator/( const float& rhs ) const
		{
			return Vertex( *this ) /= rhs;
		}

	public:
		Vec3 pos;
		Vec2 tc;
	};

	using VertexShader = DefaultVertexShader<Vertex>;
	using GeometryShader = DefaultGeometryShader<Vertex>;
	class PixelShader
	{
	public:
		void BindTexture( const std::wstring& filepath )
		{
			pTex = std::make_unique<Surface>( Surface::FromFile( filepath ) ); 
			tex_width = (float)pTex->GetWidth();
			tex_height = (float)pTex->GetHeight();
			tex_xclamp = tex_width - 1.0f;
			tex_yclamp = tex_height - 1.0f;
		}

		template<typename Input>
		Color operator()( const Input& in )
		{
			return pTex->GetPixel(
				(unsigned int)std::min( in.tc.x * tex_width + 0.5f,tex_xclamp ),
				(unsigned int)std::min( in.tc.y * tex_height + 0.5f,tex_yclamp )
			);
		}
	private:
		std::unique_ptr<Surface> pTex;
		float tex_width		= 0.0f;
		float tex_height	= 0.0f;
		float tex_xclamp	= 0.0f;
		float tex_yclamp	= 0.0f;
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
