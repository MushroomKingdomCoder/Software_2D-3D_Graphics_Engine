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
	pipe3d0(gfx, ndc, effect0, zBuffer),
	pipe3d1(gfx, ndc, effect1, zBuffer),
	/*pipe3d2(gfx, ndc, effect2, zBuffer),
	pipe3dF(gfx, ndc, effectF, zBuffer),*/
	pipe3dR(gfx, ndc, effectR, zBuffer),
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
	//effect2.VertexShader.UpdateTime(time);
	zBuffer.Clear();
	UpdateObjects(time);
	UpdateLights(time);
	UpdateCamera(time);
}

void Game::UpdateObjects(const float time)
{
	// Object0 Controls
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
	effect0.VertexShader.SetObjectTransformationMatrix(Object0.GetTransformationMatrix());
}

void Game::UpdateLights(const float time)
{
	// Directional Lighting Controls
	if (wnd.kbd.KeyIsPressed('1')) {
		d_light.RotateLightX(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('2')) {
		d_light.RotateLightY(d_rot * time);
	}
	if (wnd.kbd.KeyIsPressed('3')) {
		d_light.RotateLightZ(d_rot * time);
	}
	
	// Point Lighting Controls
	if (wnd.kbd.KeyIsPressed('T')) {
		Light.Move({ 0,0.5f * time,0 });
	}
	if (wnd.kbd.KeyIsPressed('F')) {
		Light.Move({ -0.5f * time,0,0 });
	}
	if (wnd.kbd.KeyIsPressed('G')) {
		Light.Move({ 0,-0.5f * time,0 });
	}
	if (wnd.kbd.KeyIsPressed('H')) {
		Light.Move({ 0.5f * time,0,0 });
	}
	if (wnd.kbd.KeyIsPressed('R')) {
		Light.Move({ 0,0,-0.5f * time });
	}
	if (wnd.kbd.KeyIsPressed('Y')) {
		Light.Move({ 0,0,0.5f * time });
	}
	p_light.SetPosition(Light.GetPosition());
	effectL.VertexShader.SetObjectTransformationMatrix(Light.GetTransformationMatrix());
}

void Game::UpdateCamera(const float time)
{
	bool camera_was_updated = false;
	if (wnd.mouse.LeftIsPressed() && MouseIsEngaged == false) {
		start_pos = wnd.mouse.GetPos();
		MouseIsEngaged = true;
	}
	if (MouseIsEngaged) {
		cur_pos = wnd.mouse.GetPos();
		const iVector2D delta_pos = cur_pos - start_pos;
		Camera.RotateX(delta_pos.Y * vTrack * time * float(M_PI / 180.0f) * cam_rotation_spd);
		Camera.RotateY(delta_pos.X * hTrack * time * float(M_PI / 180.0f) * cam_rotation_spd);
		start_pos = cur_pos;
		camera_was_updated = true;
		if (!wnd.mouse.LeftIsPressed()) {
			MouseIsEngaged = false;
		}
	}
	if (wnd.kbd.KeyIsPressed('I')) {
		Camera.Move(fVector3D(0, 1, 0) * time * cam_pan_spd);
		camera_was_updated = true;
	}
	if (wnd.kbd.KeyIsPressed('J')) {
		Camera.Move(fVector3D(-1, 0, 0) * time * cam_pan_spd);
		camera_was_updated = true;
	}
	if (wnd.kbd.KeyIsPressed('K')) {
		Camera.Move(fVector3D(0, -1, 0) * time * cam_pan_spd);
		camera_was_updated = true;
	}
	if (wnd.kbd.KeyIsPressed('L')) {
		Camera.Move(fVector3D(1, 0, 0) * time * cam_pan_spd);
		camera_was_updated = true;
	}
	if (wnd.kbd.KeyIsPressed('U')) {
		Camera.Move(fVector3D(0, 0, 1) * time * cam_pan_spd);
		camera_was_updated = true;
	}
	if (wnd.kbd.KeyIsPressed('O')) {
		Camera.Move(fVector3D(0, 0, -1) * time * cam_pan_spd);
		camera_was_updated = true;
	}
	if (camera_was_updated) {
		effect0.VertexShader.SetCameraTransformation(Camera.GetCameraTransformationMatrix());
		effect1.VertexShader.SetCameraTransformation(Camera.GetCameraTransformationMatrix());
		//effect2.VertexShader.SetCameraTransformation(Camera.GetCameraTransformationMatrix());
		//effectF.VertexShader.SetCameraTransformation(Camera.GetCameraTransformationMatrix());
		effectR.VertexShader.SetCameraTransformation(Camera.GetCameraTransformationMatrix());
		effectL.VertexShader.SetCameraTransformation(Camera.GetCameraTransformationMatrix());
	}
}

void Game::ComposeFrame()
{
	pipe3d0.ProcessMesh(Object0.GetTriangleModel());
	pipe3d1.ProcessMesh(Object1.GetTriangleModel());
	//pipe3d2.ProcessMesh(Object2.GetTriangleModel());
	//pipe3dF.ProcessMesh(Floor.GetTriangleModel());
	pipe3dR.ProcessMesh(Room.GetTriangleModel());
	pipe3dL.ProcessMesh(Light.GetTriangleModel());
}


