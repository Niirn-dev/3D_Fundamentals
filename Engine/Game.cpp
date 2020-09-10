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
		angleX = wrap_angle( angleX + dTheta * dt );
	}
	if ( wnd.kbd.KeyIsPressed( 'A' ) )
	{
		angleX = wrap_angle( angleX - dTheta * dt );
	}

	if ( wnd.kbd.KeyIsPressed( 'W' ) )
	{
		angleY = wrap_angle( angleY + dTheta * dt );
	}
	if ( wnd.kbd.KeyIsPressed( 'S' ) )
	{
		angleY = wrap_angle( angleY - dTheta * dt );
	}

	if ( wnd.kbd.KeyIsPressed( 'E' ) )
	{
		angleZ = wrap_angle( angleZ + dTheta * dt );
	}
	if ( wnd.kbd.KeyIsPressed( 'D' ) )
	{
		angleZ = wrap_angle( angleZ - dTheta * dt );
	}

	if ( wnd.kbd.KeyIsPressed( 'R' ) )
	{
		offsetZ = offsetZ + 2.0f * dt;
	}
	if ( wnd.kbd.KeyIsPressed( 'F' ) )
	{
		offsetZ = std::max( 2.0f,offsetZ - 2.0f * dt );;
	}
}

void Game::ComposeFrame()
{
	auto list = c.GetTriangleList();

	auto rot =
		Mat3::RotationX( angleX ) *
		Mat3::RotationY( angleY ) *
		Mat3::RotationZ( angleZ );
	for ( auto& v : list.vertices )
	{
		v *= rot;
		v.z += offsetZ;
	}

	for ( size_t i = 0; i < list.indices.size() / 3; ++i )
	{
		const Vec3& v0 = list.vertices[list.indices[3 * i]];
		const Vec3& v1 = list.vertices[list.indices[3 * i + 1]];
		const Vec3& v2 = list.vertices[list.indices[3 * i + 2]];

		if ( (v1 - v0) % (v2 - v0) * v0 >= 0.0f )
		{
			list.cullingMask[i] = false;
		}
	}

	for ( auto& v : list.vertices )
	{
		pbs.Transform( v );
	}

	const Color c[] = {
		Colors::White,
		Colors::Red,
		Colors::Cyan,
		Colors::Gray,
		Colors::Green,
		Colors::Blue,
		Colors::Yellow
	};

	for ( size_t i = 0; i < list.indices.size() / 3; ++i )
	{
		if ( list.cullingMask[i] )
		{
			gfx.DrawTriangle(
				list.vertices[list.indices[3 * i]],
				list.vertices[list.indices[3 * i + 1]],
				list.vertices[list.indices[3 * i + 2]],
				c[i % std::size( c )] );
		}
	}
}