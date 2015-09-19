#include "SpawnMachine.h"

SpawnMachine::SpawnMachine(int mode = DROPDICE, int interval = 1000)
{
	mMode = mode;
	mInterval = interval;
	mTimerCont = true;
	mMaxDice = 3;
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
	mTimerCont = false;
	mTimerThread.join();
	mDie.DeleteAllInstances();
}

void SpawnMachine::DrawAllDice()
{
	for (int i = 0; i < mDie.mInstances.size(); i++)
	{
		mDie.DrawInstance(mDie.mInstances[i]);
	}
}

// private methods
void SpawnMachine::Timer()
{
	std::cout << "Timer Start" << std::endl;
	mStartTime = Clock::now();
	while (mTimerCont)	// mTimerCont has no mutex lock
	{
		std::cout << "the time now is " 
			<< std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mStartTime).count()
			<< "milliseconds" << std::endl;
		Spawn();
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