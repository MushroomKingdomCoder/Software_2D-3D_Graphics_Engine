#include "TextFont.h"

TextFont::TextFont(std::string filename, int chW, int chH, int nR, int nC, const Color CKey)
	:
	FontFile(filename),
	ChromaKey(CKey),
	CharWidth(chW),
	CharHeight(chH),
	nRows(nR),
	nColumns(nC)
{}

void TextFont::WriteText(iVector2D pos, std::string text, Graphics& gfx, Color textColor)
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

iCRect TextFont::GetCharRect(char c)
{
	c -= ' ';
	const iVector2D Index = { c % nColumns * CharWidth,int(c / nColumns * CharHeight) };
	return iCRect(Index, CharWidth, CharHeight);
}