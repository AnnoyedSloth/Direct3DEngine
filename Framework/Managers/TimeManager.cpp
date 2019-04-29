#include "TimeManager.h"

TimeManager::TimeManager()
{
	deltaTime = 0;
}

TimeManager::~TimeManager()
{

}

void TimeManager::calculateDeltaTime()
{
	current = std::chrono::system_clock::now();
	deltaTime = ((std::chrono::duration<float>)(current - start)).count();

	start = std::chrono::system_clock::now();
}