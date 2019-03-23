# CPU Scheduler Simulation

This program simulates a monothreaded CPU scheduler. 
It runs simulated processes using threads and controls access to the process work through a mutex lock.

The first line of the input file contains the scheduler time quantum integer. This is the amount of time to be split and allocated to equally to each User
The schedueler time quantum mus tbe split between each user equally, and further split between each user process equally.

The following lines of the input file specify a user by a character (ex: A) and the number of processes that user retains, followed by each process' ready time and service time on seperate lines. 

HOW TO RUN PROGRAM FROM COMMAND LINE:

[Path to executible(.exe) file] [Directory where input is located] [Directory where you want output to be generated]

Ex: C:\Users\Public\files\Scheduler.exe C:\Users\Public\files\input.txt C:\Users\Public\files\output.txt
