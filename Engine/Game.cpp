/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
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
#include "MainWindow.h"
#include "Game.h"
#include "SolidCubeScene.h"
#include "TexCubeScene.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	cubeTexture( Surface::FromFile( L"Textures\\stone_wall_512x512.png" ) )
{
	scenes.push_back( std::make_unique<SolidCubeScene>( pst,1.0f ) );
	scenes.push_back( std::make_unique<TexCubeScene>( pst,1.0f,cubeTexture ) );
	curScene = scenes.begin();
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 1.0f / 60.0f;

	( *curScene )->Update( wnd.kbd,wnd.mouse,dt );

	if ( !wnd.kbd.KeyIsEmpty() )
	{
		auto e = wnd.kbd.ReadKey();
		if ( e.IsPress() && e.GetCode() == VK_TAB )
		{
			CycleScenes();
		}
	}
}

void Game::CycleScenes()
{
	++curScene;
	if ( curScene == scenes.end() )
	{
		curScene = scenes.begin();
	}
}

void Game::ComposeFrame()
{
	( *curScene )->Draw( gfx );
}