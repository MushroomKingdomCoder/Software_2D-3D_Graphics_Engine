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
#include "Vector2D.h"
#include "Timer.h"
#include "SpriteEffects.h"
#include "PulsatingStar.h"
#include "ScreenBuffer.h"
#include "PolyLine.h"
#include "Camera.h"
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
	void GenerateNewStar();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	//Constant Objects
	Timer Clock;
	Camera camera;
	ScreenBuffer Screen;
	fVector2D MousePos;
	fVector2D MousePos_Old;
	/********************************/
	/*  User Variables              */
	/********************************/
	static constexpr int nStars = 500;
	static constexpr int xMin = -8000;
	static constexpr int xMax = 8000;
	static constexpr int yMin = -6000;
	static constexpr int yMax = 6000;
	static constexpr int minFlares = 4;
	static constexpr int maxFlares = 9;
	static constexpr float minRadius = 10.0f;
	static constexpr float maxRadius = 150.0f;
	static constexpr float minScale = 1.25f;
	static constexpr float maxScale = 5.0f;
	static constexpr unsigned char minColor = 0;
	static constexpr unsigned char maxColor = 255;
	static constexpr int minBlinkSpd = 64;
	static constexpr int maxBlinkSpd = 512;
	static constexpr float minGrowthSpd = 1.0f;
	static constexpr float maxGrowthSpd = 3.0f;
	static constexpr float minSpinSpd = -M_PI / 4.0f;
	static constexpr float maxSpinSpd = M_PI / 4.0f;

	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> XPOS{ xMin,xMax };
	std::uniform_int_distribution<int> YPOS{ yMin,yMax };
	std::uniform_int_distribution<int> FLR{ minFlares,maxFlares };
	std::uniform_real_distribution<float> RAD{ minRadius,maxRadius };
	std::uniform_real_distribution<float> SCL{ minScale,maxScale };
	std::uniform_int_distribution<int> COL{ minColor,maxColor };
	std::uniform_int_distribution<int> BSPD{ minBlinkSpd,maxBlinkSpd };
	std::uniform_real_distribution<float> GSPD{ minGrowthSpd,maxGrowthSpd };
	std::uniform_real_distribution<float> SSPD{ minSpinSpd,maxSpinSpd };

	std::vector<PulsatingStar> Stars;
	fVector2D pos;
	float i_rad;
	float o_rad;
	float scale;
	fRect newbie;

	//Camera
	static constexpr int panSpd = 60;
	static constexpr float spinSpd = M_PI / 16.0f;
};