#include "TimeManager.h"

TimeManager::TimeManager()
{
	deltaTime = 0;
}

TimeManager::~TimeManager()
{

}

void TimeManager::CalculateDeltaTime()
{
	current = std::chrono::system_clock::now();
	deltaTime = (start - current).count();

	start = std::chrono::system_clock::now();
}