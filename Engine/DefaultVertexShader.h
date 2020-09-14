#pragma once

#include "Vec3.h"
#include "Mat3.h"

template<class Vertex>
class DefaultVertexShader
{
public:
	using Output = Vertex;

	Output operator()( const Vertex& v )
	{
		return { v.pos * rotation + translation,v };
	}
	void BindRotation( const Mat3& rot )
	{
		rotation = rot;
	}
	void BindTranslation( const Vec3& trans )
	{
		translation = trans;
	}
private:
	Mat3 rotation = Mat3::Identity();
	Vec3 translation = { 0.0f,0.0f,0.0f };
};
