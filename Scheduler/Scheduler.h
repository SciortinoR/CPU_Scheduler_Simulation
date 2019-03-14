#pragma once

#include <sstream>
#include <mutex>

#include "User.h"
#include "Process.h"

class Scheduler
{
public:

	// Constructors/Destructors
	Scheduler(int quantum);
	~Scheduler();

	// Setters

	// Getters
	int getTimeQuantum() const;
	double getCurrentTime() const;
	std::vector<User>& getUserList();
	std::stringstream& getLogBuffer();
	std::vector<Process*>& getReadyProcessList();

	// User Manipulation
	void resetAllUsers();
	void addUser(const User& user);
	void removeUser(const User& user);

	// Ready Process Manipulation
	void removeReady();
	void addReady();
	
	// Scheduler Tasks
	void cycle();
	void allocateReadyUserTime();
	void log(const Process& process);
	void updateSchedulerTime(double time);
	void sortProcesses();
	void clearVectors();
	void spawnThreads(Process* process);

	// Process Work 
	void doWork(Process* process);

private:

	// Member Variables
	double currentTime;
	int totalCycles;
	const int timeQuantum;
	std::mutex lock;

	// Output Buffer
	std::stringstream logBuffer;

	// Lists
	std::vector<User> userList;
	std::vector<User*> readyUserList;
	std::vector<Process*> readyProcessList;
	std::vector<std::thread> threadProcesses;
};

