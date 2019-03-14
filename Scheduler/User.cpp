#include <algorithm>
#include <iostream>

#include "User.h"

User::User(char Id) :
	id(Id), readyStatus(false), numProcesses(0), timeAllocated(0.0)
{
}

User::~User()
{
}

char User::getId() const
{
	return id;
}

bool User::getReadyStatus()
{
	return readyStatus;
}

int User::getNumProcesses() const
{
	return numProcesses;
}

double User::getTimeAllocated() const
{
	return timeAllocated;
}

std::vector<Process>& User::getProcessList()
{
	return processes;
}

std::vector<Process*>& User::getReadyProcesses()
{
	return readyProcesses;
}

void User::setReadyStatus(bool status)
{
	readyStatus = status;
}

void User::setNumProcesses(int num)
{
	numProcesses = num;
}

void User::setTimeAllocated(double allocation)
{
	timeAllocated = allocation;
}

void User::addProcess(const Process& process)
{
	processes.push_back(process);
}

void User::removeProcess(const Process& process)
{
	if (process.getStatus() == Process::finished)
	{
		// Removes specific process from User's process ist
		processes.erase(std::remove(processes.begin(), processes.end(), process), processes.end());
	}
}

void User::processTimeAllocation()
{
	double processAllocation = timeAllocated / readyProcesses.size();
	for (auto& process : readyProcesses)
	{
		double processTimeRemaining = process->getRemainingTime();

		// If process allocation larger than time remaining
		if (processAllocation > processTimeRemaining)
		{
			// Set allocation to time remaining
			process->setTimeAllocated(processTimeRemaining);
		}
		else
		{
			// Otherwise set allocation to process allocation
			process->setTimeAllocated(processAllocation);
		}
	}
}

void User::incrementNumProcesses()
{
	numProcesses++;
}

bool operator ==(const Process& p1, const Process& p2)
{
	return ((p1.getId() == p2.getId()) && (p1.getUserId() == p2.getUserId()));
}
