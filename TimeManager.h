#pragma once
#include <chrono>

class TimeManager
{
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point current;

public:
	TimeManager();
	virtual ~TimeManager();

	void calculateDeltaTime();

	float deltaTime;


};