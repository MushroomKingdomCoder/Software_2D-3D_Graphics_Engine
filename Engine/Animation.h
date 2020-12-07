#pragma once
#include "Sprite.h"
#include "CRect.h"
#include "Vector.h"
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
	void Play()
	{
		++curFrame;
		frameTime = 0.0f;
		if (curFrame >= nFrames) {
			curFrame = 0;
		}
	}
	
public:
	Animation(const Sprite& sSheet, const iVector2D& sSize, const iVector2D sIndex, const int nFrames, const float time, const Color CKey = Colors::Magenta)
		:
		SpriteSheet(sSheet),
		SpriteSize(sSize),
		startIndex(sIndex),
		nFrames(nFrames),
		holdTime(time),
		ChromaKey(CKey)
	{
		for (int i = 0; i < nFrames; ++i) {
			Frames.emplace_back(iCRect(startIndex.X + i * SpriteSize.X, startIndex.Y, startIndex.X + (i + 1) * SpriteSize.X, startIndex.Y + SpriteSize.Y));
		}
	}
	void Update(const float SecondsEllapsed)
	{
		frameTime += SecondsEllapsed;
		while (frameTime >= holdTime) {
			Play();
			frameTime -= holdTime;
		}
	}
	void Draw(const iVector2D& POS, Graphics& gfx, std::function<void(Graphics&, int, int, Color, Color, Color)>FX, Color aux)
	{
		gfx.DrawSprite(POS.X, POS.Y, SpriteSheet, Frames[curFrame], FX, ChromaKey, aux);
	}
};
