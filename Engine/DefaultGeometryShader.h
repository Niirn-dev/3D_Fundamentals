#pragma once

#include "IndexedTriangleList.h"
#include "Triangle.h"
#include <optional>

template<class V>
class DefaultGeometryShader
{
public:
	using Output = V;

	Triangle<Output> operator()( const V& v0,const V& v1,const V& v2,size_t id ) const
	{
		return { v0,v1,v2 };
	}
};
