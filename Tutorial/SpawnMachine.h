#pragma once
#ifndef SPAWNMACHINE
#define SPAWNMACHINE

#include <chrono>
#include <thread>
#include "Die.h"

typedef std::chrono::high_resolution_clock Clock;

class SpawnMachine
{
public:
	enum mode {DROPDICE, RAY, STACK};
	int mMode;
	int mInterval;
	int mMaxDice;
	std::chrono::time_point<Clock> mStartTime;
	Die mDie;

	SpawnMachine(int, int);
	~SpawnMachine();

	void Start();
	void Stop();
	void DrawAllDice();

private:
	std::thread mTimerThread;
	bool mTimerCont;	// no mutex lock for this variable
	void Timer();
	void Spawn();
	void Settings();
};

#endif //SPAWNMACHINE