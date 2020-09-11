#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class Scene
{
public:
	virtual ~Scene() = default;
	virtual void Update( Keyboard& kbd,Mouse& mouse,float dt ) = 0;
	virtual void Draw() = 0;
};
