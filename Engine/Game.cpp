/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
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
#include <vector>
#include <algorithm>

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	zBuffer(Graphics::ScreenWidth, Graphics::ScreenHeight),
	pipe3d(gfx, ndc, pxlS, zBuffer),
	pipe3dn(gfx, ndc, pxlSn, zBuffer)
{
	Clock.Start();
	pipe3d.BindRotation(Cube0.GetRotationMatrix());
	pipe3d.BindTranslation(Cube0.GetPosition());

	pipe3dn.BindRotation(Cube1.GetRotationMatrix());
	pipe3dn.BindTranslation(Cube1.GetPosition());
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
	zBuffer.Clear();
	const float time = Clock.GetEllapsed();
	if (wnd.kbd.KeyIsPressed('Q')) {
		Cube0.RotateX(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('W')) {
		Cube0.RotateY(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('E')) {
		Cube0.RotateZ(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('A')) {
		Cube0.RotateX(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		Cube0.RotateY(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		Cube0.RotateZ(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN)) {
		Cube0.Move({ 0,0,-0.5f * time });
	}
	if (wnd.kbd.KeyIsPressed(VK_UP)) {
		Cube0.Move({ 0,0,0.5f * time });
	}

	if (wnd.kbd.KeyIsPressed('R')) {
		Cube1.RotateX(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('T')) {
		Cube1.RotateY(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('Y')) {
		Cube1.RotateZ(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('F')) {
		Cube1.RotateX(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('G')) {
		Cube1.RotateY(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('H')) {
		Cube1.RotateZ(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed(VK_SUBTRACT)) {
		Cube1.Move({ 0,0,-0.5f * time });
	}
	if (wnd.kbd.KeyIsPressed(VK_ADD)) {
		Cube1.Move({ 0,0,0.5f * time });
	}
}

void Game::ComposeFrame()
{
	pipe3d.ProcessObject3D(Cube0.GetTriangleModel());
	pipe3dn.ProcessObject3D(Cube1.GetTriangleModel());
}


