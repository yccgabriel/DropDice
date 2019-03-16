#pragma once
#ifndef SPAWNMACHINE
#define SPAWNMACHINE

#include <chrono>
#include <atomic>
#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include "Die.h"

// shared variables
extern unsigned short tasks;
extern std::mutex tasks_mutex;
extern std::atomic<bool> timerCont;

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

	SpawnMachine(int mode = DROPDICE, int interval = 1000);
	~SpawnMachine();

	void Start();
	void Stop();
	void DrawAllDice();
	void CheckSpawn();

private:
	std::thread mTimerThread;
	void Timer();
	void Spawn();
	void Settings();
};

#endif //SPAWNMACHINE