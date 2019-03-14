#pragma once

#include <vector>
#include <string>	

#include "Process.h"

class User
{
public:

	// Constructors/Destrcutors
	User(char Id);
	~User();

	// Getters
	char getId() const;
	bool getReadyStatus();
	int getNumProcesses() const;
	double getTimeAllocated() const;
	std::vector<Process>& getProcessList();
	std::vector<Process*>& getReadyProcesses();

	// Setters
	void setReadyStatus(bool status);
	void setNumProcesses(int num);
	void setTimeAllocated(double allocation);
	
	// Process Manipulation
	void addProcess(const Process& process);
	void removeProcess(const Process& process);
	void processTimeAllocation();
	void incrementNumProcesses();

private:

	// Member Variables
	char id;
	bool readyStatus;
	int numProcesses;
	double timeAllocated;
	std::vector<Process> processes;
	std::vector<Process*> readyProcesses;
};

