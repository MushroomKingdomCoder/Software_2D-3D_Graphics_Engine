#pragma once
#include <chrono>

class Timer {
private:
	std::chrono::steady_clock::time_point Stop;

public:
	void Start()
	{
		using namespace std::chrono;
		Stop = steady_clock::now();
	}
	float GetEllapsed()
	{
		using namespace std::chrono;
		auto Start = Stop;
		Stop = steady_clock::now();
		duration<float> SecondsEllapsed = Stop - Start;
		return SecondsEllapsed.count();
	}
};