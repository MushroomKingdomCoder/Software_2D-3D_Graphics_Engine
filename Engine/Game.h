/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Character.h"
#include "TextFont.h"
#include "Rect.h"
#include "Vector.h"
#include "Timer.h"
#include "SpriteEffects.h"
#include "Indexer3D.h"
#include "ScreenBuffer.h"
#include "NDCBuffer.h"
#include "PolyLine.h"
#include "Object3D.h"
#include "PixelShaders.h"
#include "VertexShaders.h"
#include "GeometryShaders.h"
#include "Lighting.h"
#include "Effect3D.h"
#include "Pipeline3D.h"
#include "Camera3D.h"
#include "ZBuffer.h"
#include "Camera.h"
#include "Math.h"
#include <random>

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	void UpdateObjects(const float time);
	void UpdateLights(const float time);
	void UpdateCamera(const float time);
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/
	static constexpr float aspect_ratio = 1.7777777777f;
	static constexpr float hFOV = 100.0f;
	static constexpr float vFOV = hFOV / aspect_ratio;
	static constexpr float hTrack = hFOV / Graphics::ScreenWidth;
	static constexpr float vTrack = vFOV / Graphics::ScreenHeight;
	static constexpr float cam_rotation_spd = 100.0f;
	static constexpr float cam_pan_spd = 1.0f;
	// Camera
	bool MouseIsEngaged = false;
	iVector2D start_pos;
	iVector2D cur_pos;
	// *******
	const fMatrix3Dplus Projection = fMatrix3Dplus::HFOVProjection(hFOV, aspect_ratio, 0.5f, 50);
	Timer Clock;
	ZBuffer zBuffer;
	NDCBuffer ndc;
	PointLight p_light;
	DirectionalLight d_light;
	const std::vector<Light*> lights = { &p_light, &d_light };
	Camera3D Camera;

	// Object 0
	tObject3D Object0 = tObject3D::MakeSkinnedSphere( 1, 15, 15, { 0,0,6 }).AddNormals();
	pptEFFECT_ONLY effect0{ {"pokeball0.bmp",lights},{Object0.GetTransformationMatrix(), Projection},{} };
	PIPE_pptONLY pipe3d0;

	// Object 1
	tObject3D Object1 = tObject3D::MakeSkinnedCube(2, { -4,0.25f,4 });
	pptEFFECT_ONLY effect1{ {"wood.bmp",lights},{Object1.GetTransformationMatrix(), Projection},{} };
	PIPE_pptONLY pipe3d1;

	// Object 2
	//tObject3D Object2 = tObject3D::MakeTeselatedSkinnedPlane({ 20,8 }, 2.5f, 1, { 5,-4,4 });
	//PPT_SW_EFFECT effect2{ {"USflag1.bmp",lights},{Object2.GetTransformationMatrix(), Projection, 0.05f, 5, 10},{} };
	//PIPE_PPT_SW pipe3d2;

	// Floor
	//tObject3D Floor = tObject3D::MakeSkinnedPlane(20, 20, { 0,-9.99f,0 });
	//pptEFFECT_ONLY effectF{ {"floor0.bmp",lights},{Floor.GetTransformationMatrix(), Projection},{} };
	//PIPE_pptONLY pipe3dF;

	// Room
	tObject3D Room = tObject3D::MakeHollowTexturedCube(20, { 0,0,0 });
	pptEFFECT_ONLY effectR{ {"wall1.bmp",lights},{Room.GetTransformationMatrix(), Projection},{} };
	PIPE_pptONLY pipe3dR;
	
	// graphical point-light object
	mObject3D Light = mObject3D::MakeSphere(0.05f, 1, { 0,0,3 });
	mEFFECT_ONLY effectL{ {Colors::White},{Light.GetTransformationMatrix(), Projection},{} };
	PIPE_mONLY pipe3dL;

	// control vars
	static constexpr float d_rot = M_PI;
};