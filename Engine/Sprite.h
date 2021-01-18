#pragma once
#include "Colors.h"
#include "CRect.h"
#include <string>
#include <memory>
#include <vector>

class Sprite
{
private:
	std::vector<Color> Image;
	int Width = 0;
	int Height = 0;

public:
	Sprite() = default;
	Sprite(int w, int h);
	Sprite(const Sprite& s) = default;
	Sprite(Sprite&& s) = default;
	Sprite(const std::string& bmpfile);
	Sprite& operator =(const Sprite& s) = default;
	Sprite& operator =(Sprite&& s) = default;
	void PutPixel(unsigned int x, unsigned int y, Color c);
	Color GetPixel(unsigned int x, unsigned int y) const;
	int getWidth() const;
	int getHeight() const;
	iCRect getRect() const;
};