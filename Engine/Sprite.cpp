#include <fstream>
#include <assert.h>
#include <iostream>
#include "Sprite.h"
#include "ChiliWin.h"

Sprite::Sprite(int w, int h)
	:
	Width(w),
	Height(h)
{
	const int Size = Width * Height;
	Image.resize(Size);
	for (int i = 0; i < Size; ++i) {
		Image[i] = Color(0, 0, 0);
	}
}

Sprite::Sprite(const std::string& bmpfile)
{
	try
	{
		std::ifstream bmp(bmpfile, std::ios::binary);
		if (!bmp) {
			throw std::runtime_error("Failed to load bitmap file!");
		}

		BITMAPFILEHEADER bmpFile;
		bmp.read(reinterpret_cast<char*>(&bmpFile), sizeof(bmpFile));

		BITMAPINFOHEADER bmpInfo;
		bmp.read(reinterpret_cast<char*>(&bmpInfo), sizeof(bmpInfo));

		if (!bmpInfo.biCompression == BI_RGB) {
			throw std::runtime_error("Invalid bitmap compression! RGB ONLY!");
		}
		const bool is32bit = bmpInfo.biBitCount == 32;
		if (!((bmpInfo.biBitCount == 24) || is32bit)) {
			throw std::runtime_error("Invalid bitdepth! 24 or 32 bit files only!");
		}

		Width = bmpInfo.biWidth;
		Height = std::abs(bmpInfo.biHeight);
		const int Size = Width * Height;
		Image.resize(Size);

		int startY = Height - 1;
		int endY = -1;
		int inc = -1;
		if (bmpInfo.biHeight < 0) {
			startY = 0;
			endY = Height;
			inc = 1;
		}
		int eorPadding = (4 - ((Width * 3) % 4)) % 4;
		bmp.seekg(bmpFile.bfOffBits);

		for (int y = startY; y != endY; y += inc) {
			for (int x = 0; x < Width; ++x) {
				PutPixel(x, y, Color(bmp.get(), bmp.get(), bmp.get()));
				if (is32bit) { bmp.seekg(1, std::ios::cur); }
			}
			if (!is32bit) { bmp.seekg(eorPadding, std::ios::cur); }
		}
	}
	catch (const std::exception& except)
	{
		throw except;
	}
}

void Sprite::PutPixel(int x, int y, Color c)
{
	int pxl = y * Width + x;
	Image[pxl] = c;
}

Color Sprite::GetPixel(int x, int y) const
{
	int pxl = y * Width + x;
	return Image[pxl];
}

int Sprite::getWidth() const
{
	return Width;
}

int Sprite::getHeight() const
{
	return Height;
}

iCRect Sprite::getRect() const
{
	return iCRect(0, 0, Width, Height);
}
