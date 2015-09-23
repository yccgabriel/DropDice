#include "SpawnMachine.h"

unsigned short tasks;
std::mutex tasks_mutex;
std::atomic<bool> timerCont;

SpawnMachine::SpawnMachine(int mode = DROPDICE, int interval = 1000)
{
	mMode = mode;
	mInterval = interval;
	timerCont = true;
	mMaxDice = 50;
}

SpawnMachine::~SpawnMachine()
{
}

void SpawnMachine::Start()
{
	Settings();
	if (mInterval > 0)		// which means Spawn is not once-off
		mTimerThread = std::thread(&SpawnMachine::Timer, this);
}

void SpawnMachine::Stop()
{
	timerCont = false;
	mTimerThread.join();
	mDie.DeleteAllInstances();
}

void SpawnMachine::DrawAllDice()
{
//	for (int i = 0; i < mDie.mInstances.size(); i++)
//	{
//		mDie.DrawInstance(mDie.mInstances[i]);
//	}
	for (std::deque<Instance<RectangularPrism>*>::iterator it = mDie.mInstances.begin(); it != mDie.mInstances.end(); ++it)
		mDie.DrawInstance(*it);
}

void SpawnMachine::CheckSpawn()
{
	size_t tasks_count;
	tasks_mutex.lock();
	tasks_count = tasks;
	tasks_mutex.unlock();
	while (tasks_count > 0)
	{
		Spawn();
		tasks_mutex.lock();
		tasks_count = --tasks;
		tasks_mutex.unlock();
	}
}

// private methods
void SpawnMachine::Timer()
{
	std::cout << "Timer Start" << std::endl;
	mStartTime = Clock::now();
	while (timerCont)
	{
		std::cout << "the time now is " 
			<< std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mStartTime).count()
			<< "milliseconds. Spawned dice: "
			//<< mDie.mInstances.size() + 1
			<< std::endl;
		tasks_mutex.lock();
		tasks++;
		tasks_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(mInterval));
	}
}

void SpawnMachine::Spawn()
{
	while (mDie.mInstances.size() >= mMaxDice)
		mDie.DeleteInstance(mDie.mInstances.front());
	mDie.CreateInstance();
}

void SpawnMachine::Settings()
{

}