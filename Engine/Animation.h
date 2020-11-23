#pragma once
#include "Sprite.h"
#include "CRect.h"
#include "Vector2D.h"
#include "Colors.h"
#include "Graphics.h"
#include "SpriteEffects.h"
#include <vector>

class Animation 
{
private:
	const Sprite& SpriteSheet;
	const iVector2D& SpriteSize;
	const iVector2D startIndex;
	const Color ChromaKey;
	const int nFrames;
	const float holdTime;
	std::vector<iCRect> Frames;
	int curFrame = 0;
	float frameTime = 0.0f;
private:
	void Play();
	
public:
	Animation(const Sprite& sSheet, const iVector2D& sSize, const iVector2D sIndex, const int nFrames, const float time, const Color CKey = Colors::Magenta);
	void Update(const float SecondsEllapsed);
	void Draw(const iVector2D& POS, Graphics& gfx, std::function<void(Graphics&, int, int, Color, Color, Color)>FX = SpriteEffect::Null, Color aux = Colors::White);
};
