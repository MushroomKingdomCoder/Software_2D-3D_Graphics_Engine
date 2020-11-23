#include "SpriteEffects.h"
#include "Graphics.h"

namespace SpriteEffect
{
	void Null(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux)
	{
		if (col != ckey) {
			gfx.PutPixel(x, y, col);
		}
	}

	void NonChroma(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux)
	{
		gfx.PutPixel(x, y, col);
	}

	void Monochrome(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux)
	{
		if (col != ckey) {
			gfx.PutPixel(x, y, aux);
		}
	}

	void Ghost(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux)
	{
		if (col != ckey) {
			Color tgt = gfx.GetPixel(x, y);
			gfx.PutPixel(x, y, col[tgt]);
		}
	}

	void Shade(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux)
	{
		if (col != ckey) {
			gfx.PutPixel(x, y, col[aux]);
		}
	}

	void Negative(Graphics& gfx, int x, int y, Color col, Color ckey, Color aux)
	{
		if (col != ckey) {
			gfx.PutPixel(x, y, -col);
		}
	}
}