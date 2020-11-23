#pragma once
#include "Colors.h"
class Graphics;

namespace SpriteEffect
{
	void Null(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux);
	void NonChroma(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux);
	void Monochrome(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux);
	void Ghost(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux);
	void Shade(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux);
	void Negative(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux);
}
