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
	static constexpr float hFOV = 150.0f;
	static constexpr float vFOV = hFOV * aspect_ratio;
	const fMatrix3Dplus Projection = fMatrix3Dplus::HFOVProjection(hFOV, aspect_ratio, 0.5f, 25);
	Timer Clock;
	ZBuffer zBuffer;
	NDCBuffer ndc;
	PointLight light;

	// Object 0
	tObject3D Object0 = tObject3D::MakeSkinnedSphere( 1, 15, 15, { 0,0,6 }).AddNormals();
	pptEFFECT_ONLY effect0{ {"pokeball0.bmp",light},{Object0.GetTransformationMatrix(), Projection},{} };
	PIPE_pptONLY pipe3d0;

	// Object 1
	tObject3D Object1 = tObject3D::MakeSkinnedCube(2, { -4,0.25f,4 }).AddNormals();
	pptEFFECT_ONLY effect1{ {"wood.bmp",light},{Object1.GetTransformationMatrix(), Projection}, {} };
	PIPE_pptONLY pipe3d1;
	
	// graphical point-light object
	mObject3D Light = mObject3D::MakeSphere(0.05f, 1, { 0,0,3 });
	mEFFECT_ONLY effectL{ {Colors::White},{Light.GetTransformationMatrix(), Projection},{} };
	PIPE_mONLY pipe3dL;
	// control vars
	static constexpr float d_rot = M_PI;
};