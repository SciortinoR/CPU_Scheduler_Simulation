#include <iostream>
#include <iomanip>
#include <algorithm>
#include <thread>

#include "Scheduler.h"

// Account for large input of ready Processes by spawning only max of 10 threads 
// Prevents large overhead and overload of CPU/RAM
#define MAX_SPAWNED_THREADS 10

Scheduler::Scheduler(int quantum) :
	timeQuantum(quantum), totalCycles(0), currentTime(1.0)
{
}

Scheduler::~Scheduler()
{
}

std::vector<User>& Scheduler::getUserList()
{
	return userList;
}

int Scheduler::getTimeQuantum() const
{
	return timeQuantum;
}

double Scheduler::getCurrentTime() const
{
	return currentTime;
}

std::stringstream& Scheduler::getLogBuffer()
{
	return logBuffer;
}

std::vector<Process*>& Scheduler::getReadyProcessList()
{
	return readyProcessList;
}

void Scheduler::resetAllUsers()
{
	for (auto& user : userList)
	{
		user.setReadyStatus(false);
	}
}

void Scheduler::addUser(const User& user)
{
	userList.push_back(user);
}

void Scheduler::removeUser(const User& user)
{
	userList.erase(std::remove(userList.begin(), userList.end(), user), userList.end());
}

void Scheduler::addReady()
{
	// Clear ready Users and reset all User statuses
	clearVectors();
	resetAllUsers();

	// Adds all ready Processes in system to priority queue
	for (auto& user : userList)
	{
		for (auto& process : user.getProcessList())
		{
			// Only add the Process if ready time is less than current time and status is not finished
			if ((process.getReadyTime() <= currentTime) && (process.getStatus() != Process::finished))
			{
				if (process.getStatus() == Process::pending)
				{
					process.setStatus(Process::ready);
				}
				readyProcessList.push_back(&process);
				user.getReadyProcesses().push_back(&process);
				
				if (user.getReadyStatus() == false)
				{
					// Set ready User for time allocation
					user.setReadyStatus(true);
					readyUserList.push_back(&user);
				}
			}
		}
	}
}

void Scheduler::removeReady()
{
	for (auto& process : readyProcessList)
	{
		if (process->getRemainingTime() <= 0)
		{
			// Remove selected process from ready list
			readyProcessList.erase(std::remove(readyProcessList.begin(), readyProcessList.end(), process), readyProcessList.end());

			// Remove Process from User's process list as well
			for (auto& user : userList)
			{
				if (process->getUserId() == user.getId())
				{
					user.removeProcess(*process);
				}
			}
		}
	}
}

// Cycle is the schedulers main function which goes through a full
// time quantum of work, allowing it to add ready processes, sort them, 
// and execute them, all while updating/saving their statuses and scheduler time
void Scheduler::cycle()
{
	addReady();	// Add ready Processes

	// Start of each Scheduler cycle
	while (!readyProcessList.empty())
	{
		// Add ready Processes if after first cycle
		if (totalCycles != 0)
		{
			addReady();
		}

		allocateReadyUserTime();				// Allocates time to Users and Processes
		sortProcesses();						// Sorts ready Processes by remaining time (Priority Queue)

		// Spawn threads for ready Processes
		for (unsigned int i = 0; i < readyProcessList.size() && i < MAX_SPAWNED_THREADS; ++i)
		{
			spawnThreads(readyProcessList[i]);
		}

		// Wait for all threads to update their status before continuing scheduler cycle
		for (unsigned int i = 0; i < threadProcesses.size() && i < MAX_SPAWNED_THREADS; ++i)
		{
			threadProcesses[i].join();
		}

		removeReady();			// Removes completely finished processes 
		totalCycles++;			// Increment number cycles completed
	}
}

void Scheduler::allocateReadyUserTime()
{
	double userAllocation = static_cast<double>(timeQuantum) / readyUserList.size();
	for (auto& readyUser : readyUserList)
	{
		// Set User & Process time allocation
		readyUser->setTimeAllocated(userAllocation);
		readyUser->processTimeAllocation();
	}
}

void Scheduler::log(const Process& process)
{
	logBuffer << "Time " << std::fixed << std::setprecision(2) << currentTime << "s, User " << process.getUserId() 
		<< ", Process " << process.getId() << ", " << process.getStatus() << std::endl;
}

void Scheduler::updateSchedulerTime(double time)
{
	currentTime += time;
}

void Scheduler::sortProcesses()
{
	// Sort from greatest remaining time to least
	std::sort(readyProcessList.begin(), readyProcessList.end(), [](const auto& lhs, const auto& rhs)
	{
		return lhs->getRemainingTime() > rhs->getRemainingTime();
	});
}

void Scheduler::clearVectors()
{
	for (auto& user : readyUserList)
	{
		user->getReadyProcesses().clear();
	}

	threadProcesses.clear();
	readyProcessList.clear();
	readyUserList.clear();
}

// This function will spawn each ready Process in a seperate thread
void Scheduler::spawnThreads(Process* process)
{
	threadProcesses.emplace_back(&Scheduler::doWork, this, process);
}

// This function simulates work done by each Process.
// It is locked to one Process thread at a time
// so that the Process work/info and Scheduler time/info 
// can be updated/completed accordingly 
void Scheduler::doWork(Process* process)
{
	// Process Critical Section Starts
	lock.lock();

	// Check if new Process or if already paused (Dont set "Resumed" if new process starting)
	if (process->getStatus() != Process::paused)
	{
		process->setStatus(Process::started);
	}
	else
	{
		process->setStatus(Process::resumed);
	}

	// Log stats before beginning necessary work
	log(*process);

	// DO NECESSARY WORK WITH PROCESS HERE
	std::chrono::duration<double> time(process->getTimeAllocated());			// Simulating work elapsed time
	std::this_thread::sleep_for(time);

	// Update Process/Scheduler stats
	process->updateProcess();
	updateSchedulerTime(process->getTimeAllocated());
	log(*process);

	lock.unlock();
	// Process Critical Section Ends

	// Without the mutex locks, the logged outputs will be unsynchronized!
}

bool operator ==(const User& u1, const User& u2)
{
	return u1.getId() == u2.getId();
}