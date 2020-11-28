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
	rng(rd()),
	camera(),
	Screen(gfx, camera, ndc),
	MousePos(wnd.mouse.GetPos()),
	MousePos_Old(MousePos)
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
	if (wnd.kbd.KeyIsPressed('Q')) {
		Cube.RotateX(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('W')) {
		Cube.RotateY(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('E')) {
		Cube.RotateZ(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('A')) {
		Cube.RotateX(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('S')) {
		Cube.RotateY(-d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('D')) {
		Cube.RotateZ(-d_rot * time);
	}
	//UpdateCamera(time);
}

void Game::ComposeFrame()
{
	Screen.DrawPrismOutlined(Cube, Colors::Blue, Colors::Cyan);
}




void Game::UpdateCamera(const float time)
{
	if (wnd.mouse.LeftIsPressed()) {
		MousePos_Old = MousePos;
		MousePos = fVector2D(wnd.mouse.GetPos());
		fVector2D delta_pan = MousePos - MousePos_Old;
		delta_pan.X = -delta_pan.X;
		delta_pan.Rotate(camera.TiltAngle());
		camera.Pan(delta_pan * (1.0f / camera.ZoomFactor()) * time * panSpd);
	}
	else {
		MousePos = wnd.mouse.GetPos();
	}

	auto mouse_event = wnd.mouse.Read().GetType();
	if (mouse_event == Mouse::Event::Type::WheelDown) {
		camera.Zoom_By(0.95);
	}
	if (mouse_event == Mouse::Event::Type::WheelUp) {
		camera.Zoom_By(1.05f);
	}

	if (wnd.kbd.KeyIsPressed('Q')) {
		camera.Tilt(float(M_PI * time * spinSpd));
	}
	if (wnd.kbd.KeyIsPressed('E')) {
		camera.Tilt(float(-M_PI * time * spinSpd));
	}
}

