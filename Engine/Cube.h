#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include <vector>
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "Pipeline.h"

class Cube
{
public:
	template<class V>
	static inline IndexedTriangleList<V> MakeUnfolded( float size )
	{
		const float side = size / 2.0f;

		std::vector<V> vertices;
		vertices.reserve( 14 );
		vertices.emplace_back( Vec3{ -side, side,-side },Vec2{ 0.0f, 0.0f } ); // 0
		vertices.emplace_back( Vec3{  side, side,-side },Vec2{ 1.0f, 0.0f } ); // 1
		vertices.emplace_back( Vec3{ -side,-side,-side },Vec2{ 0.0f, 1.0f } ); // 2
		vertices.emplace_back( Vec3{  side,-side,-side },Vec2{ 1.0f, 1.0f } ); // 3
		vertices.emplace_back( Vec3{  side,-side, side },Vec2{ 2.0f, 1.0f } ); // 4
		vertices.emplace_back( Vec3{  side, side, side },Vec2{ 2.0f, 0.0f } ); // 5
		vertices.emplace_back( Vec3{ -side, side, side },Vec2{ 3.0f, 0.0f } ); // 6
		vertices.emplace_back( Vec3{ -side,-side, side },Vec2{ 3.0f, 1.0f } ); // 7
		vertices.emplace_back( Vec3{ -side,-side,-side },Vec2{ 4.0f, 1.0f } ); // 8
		vertices.emplace_back( Vec3{ -side, side,-side },Vec2{ 4.0f, 0.0f } ); // 9
		vertices.emplace_back( Vec3{ -side, side,-side },Vec2{ 1.0f,-1.0f } ); // 10
		vertices.emplace_back( Vec3{ -side, side, side },Vec2{ 2.0f,-1.0f } ); // 11
		vertices.emplace_back( Vec3{ -side,-side,-side },Vec2{ 1.0f, 2.0f } ); // 12
		vertices.emplace_back( Vec3{ -side,-side, side },Vec2{ 2.0f, 2.0f } ); // 13

		std::vector<size_t> indices = {
			0,1,2,   2,1,3,
			1,4,3,   4,1,5,
			5,6,4,   4,6,7,
			6,8,7,   8,6,9,
			1,10,11, 11,5,1,
			4,13,12, 12,3,4
		};

		return { std::move( vertices ),std::move( indices ) };
	}

	template<class V>
	static inline IndexedTriangleList<V> MakePlain( float size )
	{
		const float side = size / 2.0f;

		std::vector<Vec3> pos;
		pos.reserve( 8 );
		pos.emplace_back( -side, side,-side ); // 0
		pos.emplace_back(  side, side,-side ); // 1
		pos.emplace_back( -side,-side,-side ); // 2
		pos.emplace_back(  side,-side,-side ); // 3
		pos.emplace_back(  side,-side, side ); // 4
		pos.emplace_back(  side, side, side ); // 5
		pos.emplace_back( -side, side, side ); // 6
		pos.emplace_back( -side,-side, side ); // 7

		std::vector<V> vertices;
		vertices.resize( pos.size() );
		for ( size_t i = 0; i < vertices.size(); ++i )
		{
			vertices[i].pos = pos[i];
		}

		std::vector<size_t> indices = {
			0,1,2, 2,1,3,
			1,4,3, 4,1,5,
			5,6,4, 4,6,7,
			6,2,7, 2,6,0,
			1,0,6, 6,5,1,
			4,7,2, 2,3,4
		};

		return { std::move( vertices ),std::move( indices ) };
	}

	template<class V>
	static inline IndexedTriangleList<V> MakeIndividualFaces( float size )
	{
		const float side = size / 2.0f;

		std::vector<Vec3> pos;
		pos.reserve( 24 );
		pos.emplace_back( -side,side,-side );	// 0
		pos.emplace_back( side,side,-side );	// 1
		pos.emplace_back( -side,-side,-side );	// 2
		pos.emplace_back( side,-side,-side );	// 3
		pos.emplace_back( side,side,-side );	// 4
		pos.emplace_back( side,side,side );		// 5
		pos.emplace_back( side,-side,-side );	// 6
		pos.emplace_back( side,-side,side );	// 7

		pos.emplace_back( side,side,side );		// 8
		pos.emplace_back( -side,side,side );	// 9
		pos.emplace_back( side,-side,side );	// 10
		pos.emplace_back( -side,-side,side );	// 11
		pos.emplace_back( -side,side,side );	// 12
		pos.emplace_back( -side,side,-side );	// 13
		pos.emplace_back( -side,-side,side );	// 14
		pos.emplace_back( -side,-side,-side );	// 15

		pos.emplace_back( -side,side,-side );	// 16
		pos.emplace_back( -side,side,side );	// 17
		pos.emplace_back( side,side,-side );	// 18
		pos.emplace_back( side,side,side );		// 19
		pos.emplace_back( side,-side,-side );	// 20
		pos.emplace_back( side,-side,side );	// 21
		pos.emplace_back( -side,-side,-side );	// 22
		pos.emplace_back( -side,-side,side );	// 23

		std::vector<V> vertices;
		vertices.resize( pos.size() );
		for ( size_t i = 0; i < vertices.size(); ++i )
		{
			vertices[i].pos = pos[i];
		}

		std::vector<size_t> indices = {
			0,1,2, 2,1,3,
			4,7,6, 7,4,5,
			8,9,10, 11,10,9,
			12,15,14, 15,12,13,
			16,17,18, 17,19,18,
			20,21,22, 21,23,22
		};

		return { std::move( vertices ),std::move( indices ) };
	}
};
