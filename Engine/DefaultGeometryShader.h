#pragma once

#include "IndexedTriangleList.h"
#include "Triangle.h"

template<class V>
class DefaultGeometryShader
{
public:
	using Output = V;

	Triangle<V> operator()( const V& v0,const V& v1,const V& v2 ) const
	{
		return { v0,v1,v2 };
	}
};
