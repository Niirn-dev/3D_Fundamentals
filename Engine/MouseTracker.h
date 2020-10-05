#pragma once

#include "Vec2.h"
#include <cassert>
#include <utility>

class MouseTracker
{
public:
	MouseTracker() = default;
	void Engage( Vei2 pos )
	{
		assert( !isEngaged );
		isEngaged = true;
		position = std::move( pos );
	}
	void Disengage()
	{
		assert( isEngaged );
		isEngaged = false;
	}
	[[nodiscard]] Vei2 Move( Vei2 pos )
	{
		assert( isEngaged );
		return pos - std::exchange( position,std::move( pos ) );
	}
	bool IsEngaged() const
	{
		return isEngaged;
	}

private:
	bool isEngaged = false;
	Vei2 position = { 0,0 };
};