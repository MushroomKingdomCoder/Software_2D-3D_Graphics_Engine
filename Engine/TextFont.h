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
	iCRect GetCharRect(char c);

public:
	TextFont(std::string filename, int chW, int chH, int nR, int nC, const Color CKey = Colors::Magenta);
	void WriteText(iVector2D pos, std::string text, Graphics& gfx, Color textColor = Colors::White);
};