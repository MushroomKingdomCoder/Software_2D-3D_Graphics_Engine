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
#include "IndexBuffer3D.h"
#include "ScreenBuffer.h"
#include "NDCBuffer.h"
#include "PolyLine.h"
#include "Prism.h"
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
	void UpdateCamera(const float time);
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	//Constant Objects
	Timer Clock;
	Camera camera;
	NDCBuffer ndc;
	ScreenBuffer Screen;
	fVector2D MousePos;
	fVector2D MousePos_Old;
	static constexpr int panSpd = 60;
	static constexpr float spinSpd = M_PI;
	/********************************/
	/*  User Variables              */
	/********************************/
	Prism Cube = Prism::MakeCube(1.0f, { 0,0,3 });
	static constexpr float d_rot = M_PI;

	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> COL{ 0,255 };
};