/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	ChiliMath.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include <math.h>
#include <type_traits>
#include <concepts>

constexpr double PI_D = 3.1415926535897932;
constexpr float PI = (float)PI_D;

template<typename T>
concept Squarable = requires( T a )
	{
		a * a;
	};

template<typename T>
	requires Squarable<T>
inline auto sq( const T& x )
{
	return x * x;
}

template <typename T>
inline auto wrap_angle( T angle )
{
	static_assert( std::is_arithmetic_v<T>,"Template parameter is not of an arithmetic type" );
	const T modded = (T)fmod( (double)angle,2.0 * PI_D );
	return modded > (T)PI_D ?
		modded - T( 2.0 * PI_D ) :
		modded;
}

template<typename T>
inline auto interpolate( const T& lhs,const T& rhs,float alpha )
{
	return lhs + ( rhs - lhs ) * alpha;
}