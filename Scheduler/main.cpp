#include <iostream>
#include <thread>
#include <fstream>
#include <string>

#include "Scheduler.h"
#include "Process.h"	
#include "User.h"

int main(int argc, char* argv[])
{
	// Create input/output file objects
	std::fstream inputFile(argv[1], std::ios::in);
	std::fstream outputFile(argv[2], std::ios::trunc | std::ios::out);

	// Check if input file opened properly
	if (!inputFile)
	{
		std::cout << "ERROR: While opening input file";
		return 1;
	}

	// Check if output file opened properly
	if (!outputFile)
	{
		std::cout << "ERROR: While opening output file";
		return 1;
	}
	
	// Initializing Scheduler
	std::string line;
	std::getline(inputFile, line);			// First line of file
	int quantum = std::stoi(line);			// Scheduler time quantum
	Scheduler scheduler(quantum);
	
	// Parsing of input file
	char userID;
	int userCounter = 0;
	std::size_t position;
	while (std::getline(inputFile, line))
	{
		// Find first alphabetic character
		position = line.find_first_not_of("0123456789");
		if (position != std::string::npos)
		{
			userID = line[position];
			
			// Create User and add it to Scheduler
			User user(userID);
			scheduler.addUser(user);

			userCounter++;
		}
		else
		{
			// Create Process 
			Process process(scheduler.getUserList()[userCounter - 1].getNumProcesses(), userID,
				std::stoi(std::string(1, line[0])), std::stoi(std::string(1, line[1])), 
				std::stod(std::string(1, line[1])));

			// Add Process to specified User in Scheduler
			scheduler.getUserList()[userCounter - 1].addProcess(process);

			// Increment number of Processes for User
			scheduler.getUserList()[userCounter - 1].incrementNumProcesses();
		}
	}

	// Create scheduler thread that starts the scheduling tasks
	std::thread schedulerThread(&Scheduler::cycle, &scheduler);
	schedulerThread.join();

	// Copy Scheduler log buffer to output file
	outputFile << scheduler.getLogBuffer().rdbuf();

	return 0;
}
