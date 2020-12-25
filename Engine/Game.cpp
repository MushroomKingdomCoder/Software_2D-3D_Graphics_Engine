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
	pipe3d(gfx, ndc, effect, zBuffer),
	pipe3dL(gfx, ndc, effectL, zBuffer)
{
	Clock.Start();
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
	const float time = Clock.GetEllapsed();
	//effect.VertexShader.UpdateTime(time);
	zBuffer.Clear();
	// Object Controls
	if (wnd.kbd.KeyIsPressed('Q')) {
		Object0.RotateX(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('W')) {
		Object0.RotateY(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('E')) {
		Object0.RotateZ(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('A')) {
		Object0.RotateX(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		Object0.RotateY(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		Object0.RotateZ(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed(VK_DIVIDE)) {
		Object0.Move({ 0,0,0.5f * time });
	}
	if (wnd.kbd.KeyIsPressed(VK_NUMPAD2)) {
		Object0.Move({ 0,0,-0.5f * time });
	}
	// ! experimental !
	if (wnd.kbd.KeyIsPressed(VK_NUMPAD8)) {
		Object0.Move({ 0,0.5f * time,0 });
	}
	if (wnd.kbd.KeyIsPressed(VK_NUMPAD4)) {
		Object0.Move({ -0.5f * time,0,0 });
	}
	if (wnd.kbd.KeyIsPressed(VK_NUMPAD5)) {
		Object0.Move({ 0,-0.5f * time,0 });
	}
	if (wnd.kbd.KeyIsPressed(VK_NUMPAD6)) {
		Object0.Move({ 0.5f * time,0,0 });
	}

	// Directional Lighting Controls
	//if (wnd.kbd.KeyIsPressed('1')) {
	//	light.RotateLightX(angle_wrap(d_rot * time));
	//}
	//if (wnd.kbd.KeyIsPressed('2')) {
	//	light.RotateLightY(angle_wrap(d_rot * time));
	//}
	//if (wnd.kbd.KeyIsPressed('3')) {
	//	light.RotateLightZ(angle_wrap(d_rot * time));
	//}

	// Point Lighting Controls
	if (wnd.kbd.KeyIsPressed('T')) {
		light.Move({ 0,0.5f * time,0 });
	}
	if (wnd.kbd.KeyIsPressed('F')) {
		light.Move({ -0.5f * time,0,0 });
	}
	if (wnd.kbd.KeyIsPressed('G')) {
		light.Move({ 0,-0.5f * time,0 });
	}
	if (wnd.kbd.KeyIsPressed('H')) {
		light.Move({ 0.5f * time,0,0 });
	}
	if (wnd.kbd.KeyIsPressed('R')) {
		light.Move({ 0,0,-0.5f * time });
	}
	if (wnd.kbd.KeyIsPressed('Y')) {
		light.Move({ 0,0,0.5f * time });
	}
	Light.SetPosition(light.GetPosition());
}

void Game::ComposeFrame()
{
	pipe3d.ProcessObject3D(Object0.GetTriangleModel());
	pipe3dL.ProcessObject3D(Light.GetTriangleModel());
}


