#include "stdafx.h"
#include "FPSCounter.h"


FPSCounter::FPSCounter()
{
	lastTime = system_clock::now();
}

FPSCounter::~FPSCounter()
{
}

void FPSCounter::Update()
{
	fpsStep++;
	milli_duration duration = system_clock::now() - lastTime;
	if (duration.count() > 1000.0f)
	{
		lastTime = system_clock::now();
		lastFps = fpsStep;
		fpsStep = 0;
	}
}

std::string FPSCounter::ToString()
{
	std::string str;
	str = std::to_string(lastFps);
	str += "fps";
	return str;
}

