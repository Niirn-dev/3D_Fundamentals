/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Mat2.h																				  *
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

#include "Vec3.h"
#include "Vec4.h"

template <typename T,size_t S>
class _Mat
{
public:
	_Mat&	operator=( const _Mat& rhs )
	{
		memcpy( elements,rhs.elements,sizeof( elements ) );
		return *this;
	}
	_Mat&	operator*=( T rhs )
	{
		for ( auto& row : elements )
		{
			for ( T& e : row )
			{
				e *= rhs;
			}
		}
		return *this;
	}
	_Mat	operator*( T rhs ) const
	{
		_Mat result = *this;
		return result *= rhs;
	}
	_Mat&	operator*=( const _Mat& rhs )
	{
		return *this = *this * rhs;
	}
	_Mat	operator*( const _Mat& rhs ) const
	{
		_Mat result;
		for ( size_t j = 0; j < S; j++ )
		{
			for ( size_t k = 0; k < S; k++ )
			{
				T sum = (T)0.0;
				for ( size_t i = 0; i < S; i++ )
				{
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}
	_Mat&	Transpose()
	{
		for ( size_t row = 0; row < S; ++row )
		{
			for ( size_t col = row + 1; col < S; ++col )
			{
				std::swap( elements[row][col],elements[col][row] );
			}
		}
		return *this;
	}
	_Mat	GetTransposed() const
	{
		return _Mat( *this ).Transpose();
	}
	constexpr static _Mat Identity()
	{
		if constexpr ( S == 3 )
		{
			return {
				(T)1.0,(T)0.0,(T)0.0,
				(T)0.0,(T)1.0,(T)0.0,
				(T)0.0,(T)0.0,(T)1.0
			};
		}
		else if constexpr ( S == 4 )
		{
			return {
				(T)1.0,(T)0.0,(T)0.0,(T)0.0,
				(T)0.0,(T)1.0,(T)0.0,(T)0.0,
				(T)0.0,(T)0.0,(T)1.0,(T)0.0,
				(T)0.0,(T)0.0,(T)0.0,(T)1.0,
			};
		}
		else
		{
			static_assert( false,"Bad matrix dimentions" );
		}
	}
	constexpr static _Mat Scaling( T factor )
	{
		if constexpr ( S == 3 )
		{
			return{
				factor,(T)0.0,(T)0.0,
				(T)0.0,factor,(T)0.0,
				(T)0.0,(T)0.0,factor
			};
		}
		else if constexpr ( S == 4 )
		{
			return{
				factor,(T)0.0,(T)0.0,(T)0.0,
				(T)0.0,factor,(T)0.0,(T)0.0,
				(T)0.0,(T)0.0,factor,(T)0.0,
				(T)0.0,(T)0.0,(T)0.0,(T)1.0
			};
		}
		else
		{
			static_assert( false,"Bad matrix dimentions" );
		}
	}
	constexpr static _Mat RotationZ( T theta )
	{
		const T sinTheta = sin( theta );
		const T cosTheta = cos( theta );
		if constexpr ( S == 3 )
		{
			return{
				 cosTheta, sinTheta, (T)0.0,
				-sinTheta, cosTheta, (T)0.0,
				(T)0.0,    (T)0.0,	 (T)1.0
			};
		}
		else if constexpr ( S == 4 )
		{
			return{
				 cosTheta, sinTheta, (T)0.0, (T)0.0,
				-sinTheta, cosTheta, (T)0.0, (T)0.0,
				(T)0.0,    (T)0.0,   (T)1.0, (T)0.0,
				(T)0.0,    (T)0.0,   (T)0.0, (T)1.0
			};
		}
		else
		{
			static_assert( false,"Bad matrix dimentions" );
		}
	}
	constexpr static _Mat RotationY( T theta )
	{
		const T sinTheta = sin( theta );
		const T cosTheta = cos( theta );
		if constexpr ( S == 3 )
		{
			return{
				 cosTheta, (T)0.0,-sinTheta,
				 (T)0.0,   (T)1.0, (T)0.0,
				 sinTheta, (T)0.0, cosTheta
			};
		}
		else if constexpr ( S == 4 )
		{
			return{
				cosTheta, (T)0.0,-sinTheta, (T)0.0,
				(T)0.0,   (T)1.0, (T)0.0,   (T)0.0,
				sinTheta, (T)0.0, cosTheta, (T)0.0,
				(T)0.0,   (T)0.0, (T)0.0,   (T)1.0
			};
		}
		else
		{
			static_assert( false,"Bad matrix dimentions" );
		}
	}
	constexpr static _Mat RotationX( T theta )
	{
		const T sinTheta = sin( theta );
		const T cosTheta = cos( theta );
		if constexpr ( S == 3 )
		{
			return{
				(T)1.0, (T)0.0,   (T)0.0,
				(T)0.0, cosTheta, sinTheta,
				(T)0.0,-sinTheta, cosTheta,
			};
		}
		else if constexpr ( S == 4 )
		{
			return{
				(T)1.0, (T)0.0,   (T)0.0,   (T)0.0,
				(T)0.0, cosTheta, sinTheta, (T)0.0,
				(T)0.0,-sinTheta, cosTheta, (T)0.0,
				(T)0.0, (T)0.0,   (T)0.0,   (T)1.0
			};
		}
		else
		{
			static_assert( false,"Bad matrix dimentions" );
		}
	}
	constexpr static _Mat Translation( T dx,T dy,T dz )
	{
		static_assert( S == 4,"Bad matrix dimentions. Translation only supported for 4 dimentional matrices" );
		return{
			(T)1.0,(T)0.0,(T)0.0,(T)0.0,
			(T)0.0,(T)1.0,(T)0.0,(T)0.0,
			(T)0.0,(T)0.0,(T)1.0,(T)0.0,
			  dx  ,  dy  ,  dz  ,(T)1.0
		};
	}
	constexpr static _Mat Translation( _Vec3<T> offset )
	{
		return std::move( Translation( offset.x,offset.y,offset.z ) );
	}
	constexpr static _Mat Projection( T w,T h,T n,T f )
	{
		static_assert( S == 4,"Bad matrix dimentions. Projection only supported for 4 dimentional matrices" );

		const auto zDenInv = (T)1.0 / ( f - n );
		return {
			(T)2.0 * n / w,(T)0.0,        (T)0.0,          (T)0.0,
			(T)0.0,        (T)2.0 * n / h,(T)0.0,          (T)0.0,
			(T)0.0,        (T)0.0,        f * zDenInv,     (T)1.0,
			(T)0.0,        (T)0.0,        -f * n * zDenInv,(T)0.0
		};
	}
	constexpr static _Mat ProjectionHFOV( T fov_deg,T ar,T n,T f )
	{
		static_assert( S == 4,"Bad matrix dimentions. Projection only supported for 4 dimentional matrices" );

		const T fov_rad = fov_deg * (T)PI / (T)180.0;
		const T w = (T)1.0 / std::tan( fov_rad / (T)2.0 );
		const T h = w * ar;
		const auto zDenInv = (T)1.0 / ( f - n );

		return {
			w,     (T)0.0,(T)0.0,          (T)0.0,
			(T)0.0,h,     (T)0.0,          (T)0.0,
			(T)0.0,(T)0.0,f * zDenInv,     (T)1.0,
			(T)0.0,(T)0.0,-f * n * zDenInv,(T)0.0
		};
	}
public:
	// [ row ][ col ]
	T elements[S][S];
};

template<typename T>
_Vec3<T>& operator*=( _Vec3<T>& lhs,const _Mat<T,3>& rhs )
{
	return lhs = lhs * rhs;
}
template<typename T>
_Vec3<T> operator*( const _Vec3<T>& lhs,const _Mat<T,3>& rhs )
{
	return{
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
	};
}

template<typename T>
_Vec4<T>& operator*=( _Vec4<T>& lhs,const _Mat<T,4>& rhs )
{
	return lhs = lhs * rhs;
}
template<typename T>
_Vec4<T> operator*( const _Vec4<T>& lhs,const _Mat<T,4>& rhs )
{
	return{
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0] + lhs.w * rhs.elements[3][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1] + lhs.w * rhs.elements[3][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2] + lhs.w * rhs.elements[3][2],
		lhs.x * rhs.elements[0][3] + lhs.y * rhs.elements[1][3] + lhs.z * rhs.elements[2][3] + lhs.w * rhs.elements[3][3]
	};
}

using Mat3 = _Mat<float,3>;
using Mad3 = _Mat<double,3>;
using Mat4 = _Mat<float,4>;
using Mad4 = _Mat<double,4>;