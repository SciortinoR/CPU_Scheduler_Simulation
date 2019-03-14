#pragma once

#include <string>

class Process
{
public:

	// Constructors/Destructors
	Process(int Id, char user, int ready, int service, int time);
	~Process();

	// Getters
	int getId() const;
	char getUserId() const;
	int getReadyTime() const;
	int getServiceTime() const;
	double getRemainingTime() const;
	double getTimeAllocated() const;
	std::string getStatus() const;

	// Setters
	void setRemainingTime(double time);
	void setTimeAllocated(double allocation);
	void setStatus(std::string state);

	// Process Update
	void updateProcess();

	// Class Variables
	inline static const std::string pending = "Not Ready";
	inline static const std::string ready = "Ready";
	inline static const std::string started = "Started";
	inline static const std::string resumed = "Resumed";
	inline static const std::string paused = "Paused";
	inline static const std::string finished = "Finished";

private:

	// Member Variables
	int id;
	char userId;
	int readyTime;
	int serviceTime;
	double timeRemaining;
	double timeAllocated;
	std::string status;
};

