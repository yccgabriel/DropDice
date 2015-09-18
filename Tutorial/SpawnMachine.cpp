#include "SpawnMachine.h"

SpawnMachine::SpawnMachine(int mode = DROPDICE, int interval = 1000)
{
	mMode = mode;
	mInterval = interval;
	mTimerCont = true;
	switch(mode)
	{
	case DROPDICE:
		break;
	case RAY:
		break;
	case STACK:
		break;
	}
}

SpawnMachine::~SpawnMachine()
{
}

void SpawnMachine::Start()
{
	mTimerThread = std::thread(&SpawnMachine::Timer, this);
	//if		(mMode == DROPDICE)	DropDice();
	//else if (mMode == RAY)		Ray();
	//else if (mMode == STACK)	Stack();
}

void SpawnMachine::Stop()
{
	mTimerCont = false;
	mTimerThread.join();
	mDie.DeleteAllInstances();
}

void SpawnMachine::DrawAllDice()
{
	for (int i = 0; i < mDie.m_opvInstances.size(); i++)
	{
		mDie.DrawInstance(mDie.m_opvInstances[i]);
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
	mDie.CreateInstance();
}