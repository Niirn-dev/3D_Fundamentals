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
#include "Mat2.h"
#include "Mat3.h"
#include "IndexedLineList.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	c( 1.0f )
{
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

	if ( wnd.kbd.KeyIsPressed( 'Q' ) )
	{
		angleX += dTheta * dt;
	}
	if ( wnd.kbd.KeyIsPressed( 'A' ) )
	{
		angleX -= dTheta * dt;
	}

	if ( wnd.kbd.KeyIsPressed( 'W' ) )
	{
		angleY += dTheta * dt;
	}
	if ( wnd.kbd.KeyIsPressed( 'S' ) )
	{
		angleY -= dTheta * dt;
	}

	if ( wnd.kbd.KeyIsPressed( 'E' ) )
	{
		angleZ += dTheta * dt;
	}
	if ( wnd.kbd.KeyIsPressed( 'D' ) )
	{
		angleZ -= dTheta * dt;
	}
}

void Game::ComposeFrame()
{
	auto ill = c.GetLineList();

	auto rot =
		Mat3::RotationX( angleX ) *
		Mat3::RotationY( angleY ) *
		Mat3::RotationZ( angleZ );
	for ( auto& v : ill.vertices )
	{
		v *= rot;
		v.z += 1.0f;

		pbs.Transform( v );

	}

	for ( auto i = ill.indices.begin(); i != ill.indices.end(); std::advance( i,2 ) )
	{
		gfx.DrawLine( ill.vertices[*i],ill.vertices[*std::next( i )],Colors::White );
	}
}