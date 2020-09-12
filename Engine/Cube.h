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
		vertices.emplace_back( Vec3{ -side, side,-side },Vec2{  0.0f, 0.0f } ); // 0
		vertices.emplace_back( Vec3{  side, side,-side },Vec2{  1.0f, 0.0f } );	// 1
		vertices.emplace_back( Vec3{ -side,-side,-side },Vec2{  0.0f, 1.0f } );	// 2
		vertices.emplace_back( Vec3{  side,-side,-side },Vec2{  1.0f, 1.0f } );	// 3
		vertices.emplace_back( Vec3{  side,-side, side },Vec2{  2.0f, 1.0f } );	// 4
		vertices.emplace_back( Vec3{  side, side, side },Vec2{  2.0f, 0.0f } );	// 5
		vertices.emplace_back( Vec3{ -side, side, side },Vec2{  3.0f, 0.0f } );	// 6
		vertices.emplace_back( Vec3{ -side,-side, side },Vec2{  3.0f, 1.0f } );	// 7
		vertices.emplace_back( Vec3{ -side,-side,-side },Vec2{  4.0f, 1.0f } );	// 8
		vertices.emplace_back( Vec3{ -side, side,-side },Vec2{  4.0f, 0.0f } );	// 9
		vertices.emplace_back( Vec3{ -side, side,-side },Vec2{  1.0f,-1.0f } );	// 10
		vertices.emplace_back( Vec3{ -side, side, side },Vec2{  2.0f,-1.0f } );	// 11
		vertices.emplace_back( Vec3{ -side,-side,-side },Vec2{  1.0f, 2.0f } );	// 12
		vertices.emplace_back( Vec3{ -side,-side, side },Vec2{  2.0f, 2.0f } );	// 13

		std::vector<size_t> indices = {
			0,1,2,   2,1,3,
			1,4,3,   4,1,5,
			5,6,4,   4,6,7,
			6,8,7,   8,6,9,
			1,10,11, 11,5,1,
			4,13,12, 12,3,4
		};

		return { vertices,indices };
	}
};
