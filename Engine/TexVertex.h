#pragma once

#include "Vec3.h"
#include "Vec2.h"
#include <vector>

class TexVertex
{
public:
	TexVertex( const Vec3& pos,const Vec2& tc )
		:
		pos( pos ),
		tc( tc )
	{}
	TexVertex&	operator=( const TexVertex& rhs )
	{
		pos = rhs.pos;
		tc = rhs.tc;
		return *this;
	}
	TexVertex&	operator+=( const TexVertex& rhs )
	{
		pos += rhs.pos;
		tc += rhs.tc;
		return *this;
	}
	TexVertex&	operator-=( const TexVertex& rhs )
	{
		pos -= rhs.pos;
		tc -= rhs.tc;
		return *this;
	}
	TexVertex	operator+( const TexVertex& rhs ) const
	{
		return TexVertex( *this ) += rhs;
	}
	TexVertex	operator-( const TexVertex& rhs ) const
	{
		return TexVertex( *this ) -= rhs;
	}
	TexVertex	InterpolateTo( const TexVertex& rhs,float alpha ) const
	{
		return {
			pos.InterpolateTo( rhs.pos,alpha ),
			tc.InterpolateTo( rhs.tc,alpha )
		};
	}
public:
	Vec3 pos;
	Vec2 tc;
};
