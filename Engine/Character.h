#pragma once
#include "Sprite.h"
#include "Vector.h"
#include "Animation.h"
#include "SpriteEffects.h"
#include "Graphics.h"
#include <vector>
#include <string>

class Character
{
private:
	enum Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};
private:
	Sprite Body;
	std::vector<Animation> Animations;
	Sequence CurAnimationSequence = StandingDown;
	fVector2D Pos;

public:
	Character(std::string sprite_sheet_file, iVector2D pos, const iVector2D& sSize, const int nFrames, const Color CKey = Colors::Magenta);
	void Update(const fVector2D& vel, const float SecondsEllapsed);
	void Draw(Graphics& gfx, std::function<void(Graphics&, int, int, Color, Color, Color)>FX = SpriteEffect::Null, Color aux = Colors::White);
};
