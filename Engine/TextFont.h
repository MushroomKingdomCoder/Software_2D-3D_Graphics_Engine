#pragma once
#include "Sprite.h"
#include "Colors.h"
#include "SpriteEffects.h"
#include "CRect.h"
#include "Graphics.h"
#include <string>

class TextFont 
{
private:
	Sprite FontFile;
	Color ChromaKey;
	const int CharWidth;
	const int CharHeight;
	const int nRows;
	const int nColumns;
private:
	static constexpr char firstChar = ' ';
	static constexpr char lastChar = '~';
private:
	iCRect GetCharRect(char c)
	{
		c -= ' ';
		const iVector2D Index = { c % nColumns * CharWidth,int(c / nColumns * CharHeight) };
		return iCRect(Index, CharWidth, CharHeight);
	}

public:
	TextFont(std::string filename, int chW, int chH, int nR, int nC, const Color CKey = Colors::Magenta)
		:
		FontFile(filename),
		ChromaKey(CKey),
		CharWidth(chW),
		CharHeight(chH),
		nRows(nR),
		nColumns(nC)
	{}
	void WriteText(iVector2D pos, std::string text, Graphics& gfx, Color textColor = Colors::White)
	{
		auto curPos = pos;
		for (char c : text) {
			if (c == '\n') {
				curPos.X = pos.X;
				curPos.Y += CharHeight;
				continue;
			}
			if (c > firstChar && c <= lastChar) {
				gfx.DrawSprite(curPos.X, curPos.Y, FontFile, GetCharRect(c), SpriteEffect::Monochrome, ChromaKey, textColor);
			}
			curPos.X += CharWidth;
			if (curPos.X > Graphics::ScreenWidth - CharWidth) {
				curPos.X = pos.X;
				curPos.Y += CharHeight;
			}
		}
	}
};