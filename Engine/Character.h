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
	Character::Character(std::string sprite_sheet_file, iVector2D pos, const iVector2D& sSize, const int nFrames, const Color CKey = Colors::Magenta)
		:
		Body(sprite_sheet_file),
		Pos(pos)
	{
		for (int i = (int)WalkingLeft; i < (int)StandingLeft; ++i) {
			Animations.emplace_back(Animation(Body, sSize, iVector2D(0, i * sSize.Y), nFrames, 0.1f, CKey));
		}
		for (int i = (int)StandingLeft; i < (int)Count; ++i) {
			Animations.emplace_back(Animation(Body, sSize, iVector2D(0, (i - (int)StandingLeft) * sSize.Y), 1, 1.0f, CKey));
		}
	}
	void Character::Update(const fVector2D& vel, const float SecondsEllapsed)
	{
		if (vel.X < 0.0f) {
			CurAnimationSequence = WalkingLeft;
		}
		else if (vel.X > 0.0f) {
			CurAnimationSequence = WalkingRight;
		}
		else if (vel.Y < 0.0f) {
			CurAnimationSequence = WalkingUp;
		}
		else if (vel.Y > 0.0f) {
			CurAnimationSequence = WalkingDown;
		}
		else {
			switch (CurAnimationSequence) {
			case Sequence::WalkingLeft:
				CurAnimationSequence = StandingLeft; break;
			case Sequence::WalkingRight:
				CurAnimationSequence = StandingRight; break;
			case Sequence::WalkingUp:
				CurAnimationSequence = StandingUp; break;
			case Sequence::WalkingDown:
				CurAnimationSequence = StandingDown; break;
			default:
				break;
			}
		}

		Animations[(int)CurAnimationSequence].Update(SecondsEllapsed);
		Pos += vel * SecondsEllapsed;
	}
	void Draw(Graphics& gfx, std::function<void(Graphics&, int, int, Color, Color, Color)>FX = SpriteEffect::Null, Color aux = Colors::White)
	{
		Animations[(int)CurAnimationSequence].Draw(Pos, gfx, FX, aux);
	}
};
