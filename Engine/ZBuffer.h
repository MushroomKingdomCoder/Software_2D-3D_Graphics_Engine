#pragma once
#include <memory>

class ZBuffer
{
private:
	const size_t Width;
	const size_t Height;
	std::unique_ptr<float[]> pZBuffer;

public:
	ZBuffer(const unsigned int width, const unsigned int height)
		:
		Width(width),
		Height(height)
	{
		pZBuffer = std::make_unique<float[]>(Width * Height);
	}
	void Clear()
	{
		for (int i = 0; i < Width * Height; ++i) {
			pZBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}
	bool TestAndSet(const unsigned int x, const unsigned int y, const float z)
	{
		float& curDepth = pZBuffer[y * Width + x];
		if (z < curDepth) {
			curDepth = z;
			return true;
		}
		else {
			return false;
		}
	}
};



