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
	Timer Clock;
	ZBuffer zBuffer;
	NDCBuffer ndc;
	pptObject3D Object0 = pptObject3D::GetObjectModelWithNormals(tObject3D::MakeSkinnedSphere( 1, 15, 15, { 0,0,3 }));
	PointLight light;
	pptEFFECT_ONLY effect{ {"pokeball0.bmp",light},{Object0.GetTransformationMatrix()},{} };
	PIPE_pptONLY pipe3d;
	
	mObject3D Light = mObject3D::MakeSphere(0.05f, 1, { 0,0,1 });
	mEFFECT_ONLY effectL{ {Colors::White},{Light.GetTransformationMatrix()},{} };
	PIPE_mONLY pipe3dL;
	// control vars
	static constexpr float d_rot = M_PI;
};