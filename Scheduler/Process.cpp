#include "Process.h"

Process::Process(int Id, char user, int ready, int service, int time) :
	id(Id), userId(user), readyTime(ready), serviceTime(service), timeRemaining(time), 
	timeAllocated(0.0), status("Not Ready")
{
}

Process::~Process()
{
}

int Process::getId() const
{
	return id;
}

char Process::getUserId() const
{
	return userId;
}

int Process::getReadyTime() const
{
	return readyTime;
}

int Process::getServiceTime() const
{
	return serviceTime;
}

double Process::getRemainingTime() const
{ 
	return timeRemaining;
}

double Process::getTimeAllocated() const
{
	return timeAllocated;
}

std::string Process::getStatus() const
{
	return status;
}

void Process::setRemainingTime(double time)
{
	timeRemaining = time;
}

void Process::setTimeAllocated(double allocation)
{
	timeAllocated = allocation;
}

void Process::setStatus(std::string state)
{
	status = state;
}

void Process::updateProcess()
{
	// Process time remaining is updated, based on time allocated
	timeRemaining -= timeAllocated;

	// Process status updated based on time remaining
	if (timeRemaining <= 0)
	{
		status = Process::finished;
	}
	else
	{
		status = Process::paused;
	}
}
