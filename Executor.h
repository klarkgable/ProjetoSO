#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "../../Conventions/job.h"
#include "../../Conventions/conventions.h"


#ifndef EXECUTOR_H_INCLUDED
#define EXECUTOR_H_INCLUDED

class Scheduler {
        private:
                int _shmId;
                int *_jobId;
                int _msqId;

                int _shmId2;
                int *_listLength;
                int _msqId2;

                /**
                *   @brief Parses a time string to two short ints: hours and minutes.
                *   @param timeString User input in the format of a string in the format <hours>:<minutes>.
                *   @param hours Hour value from the string.
                *   @param minutes Minute value from the string.
                */
                void parseTime(char *timeString, short *hours, short *minutes);

                /**
                *   @brief Sends a message.
                *   @param message Message to be sent.
                */
                void sendMessage(JobMessage message);

                std::vector<std::string> splitString(std::string input, std::string delimiter);

	public:
                /**
                * @brief Default constructor. Does nothing special.
                */
                Scheduler();

                /**
                * @brief Default destructor. Does nothing special.
                */
        	~Scheduler();

        	/**
                * @brief Main method.
                */
        	void main(int argc, char** argv);

        	/**
                *   @brief Creates the environment for executing a scheduling operation. Convers user input to something
                *          the program can understand.
                *   @param argv Command line used to call the program.
                */
        	void setup(char** argv);

                /**
                *   @brief Schedules a program execution with given parameters.
                *   @param hours How many hours from now should the program's execution be postponed.
                *   @param minutes How many minutes from now should the program's execution be postponed.
                *   @param times How many times should the program be executed.
                *   @param path Where is the executable of the program located.
                */
        	void schedule(short hours, short minutes, int times, std::string path);

        	/**
                *   @brief Unschedule a program execution.
                *   @param id Unique ID of the process whose execution will be cancelled.
                */
        	void unschedule(int id);

                void listJobs();

                void shutdownServer();

                /**
                *   @brief Shuts the program down.
                */
                void shutdown();
};


#endif // EXECUTOR_H_INCLUDED
