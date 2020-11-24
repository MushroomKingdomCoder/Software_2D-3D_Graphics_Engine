#pragma once
#include "PolyLine.h"
#include "Rect.h"
#include "Colors.h"
#include "Graphics.h"
#include "ScreenBuffer.h"

class PulsatingStar
{
private:
	PolyLine Star = PolyLine();
	float Radius;
private:
	float EnlargenedScale = 1.0f;
	Color PulsingColor = Colors::Yellow;
	Color CurColor = PulsingColor;
	float cScale = 0;
	bool isGrowing = true;
	int blinkSpeed = 128;
	float GrowthSpeed = 1.0f;
	float SpinSpeed = 1.0f;

public:
	PulsatingStar() = default;
	PulsatingStar(int nFlares, float i_rad, float o_rad, fVector2D pos, float es, Color c, int bs, float gs, float ss)
		:
		EnlargenedScale(es),
		PulsingColor(c),
		CurColor(PulsingColor),
		blinkSpeed(bs),
		GrowthSpeed(gs),
		SpinSpeed(ss)
	{
		Star = PolyLine::MakeStar(nFlares, i_rad, o_rad, pos);
		Radius = o_rad;
	}
	void Pulse(float time)
	{
		cScale += float(time * blinkSpeed);
		CurColor = PulsingColor[Color((int)cScale % 256, (int)cScale % 256, (int)cScale % 256)];
		
		if (isGrowing) {
			Star.ScaleBy(1.0f + (time * GrowthSpeed));
			if (std::abs(Star.GetScale()) > EnlargenedScale) {
				isGrowing = false;
			}
		}
		else {
			Star.ScaleBy(1.0f - (time * GrowthSpeed));
			if (std::abs(Star.GetScale()) < 1.0f) {
				isGrowing = true;
			}
		}

		Star.Rotate(float(M_PI * time * SpinSpeed));
	}
	void Draw(ScreenBuffer& screen)
	{
		screen.DrawPolyLine(Star, CurColor);
	}
	fRect GetMaxRect() const
	{
		auto star_pos = Star.GetPos();
		auto MaxLength = Radius * EnlargenedScale;
		return fRect(fVector2D(star_pos.X - MaxLength, star_pos.Y + MaxLength), MaxLength);
	}
};
