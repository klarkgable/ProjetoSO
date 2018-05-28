#include <iostream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
//#include <sys/shm.h>
#include <list>
#include <vector>
#include "Estrutura.h"



// Error macros
#define SUCCESS 0 /// Global Success
#define ERROR_ARGS -1 /// Global Argument Error
#define ERROR_FUNC -2 /// Global Functionality Error
#define ERROR_SHM 1 /// Shared Memory Error
#define ERROR_MSQ 2 /// Message Queue Error

/// IPC macros
#define SHM_FLAGS 0644 /// Shared Memory Flag
#define SHM_KEY 0x111141 /// Shared Memory Key
#define MSQ_FLAGS 0644 /// Message Queue Flag
#define MSQ_KEY 0x159560 /// Message Queue Key
#define MSG_TYPE 1

/// Messages
#define MSG_CANCEL "Cancel" /// Message to cancel a job
#define MSG_REQLIST "List" /// Message to request the jobs list

///Schedule Macros
#define QUANTUM = 10;



#ifndef ESCALONADOR_H_INCLUDED
#define ESCALONADOR_H_INCLUDED

class JobExecution {

    private:

        pid_t _pid = -1;
        time_t receivedTime;
        double timeToNextExecution;


    public:
        int remaingTimes = _job._times;
        job _job;
        void setReceivedTime();
        time_t getReceivedTime();
        void setJob(job rcvJob);
        Job getJob();
        int getJobID();
        void setPID(pid_t pid);
        pid_t getPID();
        JobExecution();
        void resetTimeToNextExecution();
        void decreaseTimeToNextExecution(double dTime);
        double getTimeToNextExecution();
    bool operator==(const JobExecution &other) const;
};

/*typedef struct job {
    int _jobId = -1; /// Unique
    std::string _programName; /// Path
    int _hour;
    int _minute;
    int _times;
} Job;

typedef struct jobmsg {
	long _mtype;
	char _job[100];
} JobMessage;



/**
* Classe do escalonador
*/
class Server {
	private:
            int _shmId;
            int _shmId2;
            int *_listLenght;
            int *_jobId;
            int _msqId;
            int _msqId2;

	public:
            std::list<JobExecution> _newJobs; //Received Jobs
            std::list<JobExecution> _jobsInExecution;
            JobExecution _jobInExecution;

            /**
            * @brief Default constructor. Does nothing special.
            */
            Server();

            /**
            * @brief Default destructor. Does nothing special.
            */
    		~Server();

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
            *   @brief Shuts the program down. You might want to do this on the destructor, I don't know.
            */
            void shutdown();

            /**
            *   @brief Receive message sent by scheduler. Wait until the message is available.
            */
            void rcvMessage();

            /**
            *   @brief Split string into subtrings.
            *   @param input String that will be broken
            *   @param delimiter delimiter of each substring
            *   @return returns a vector with substrings
            */
            std::vector<std::string> splitString(std::string input, std::string delimiter);

            /**
            *   @brief Parse a message vector in Job.
            *   @param message Vector that contains job values.
            *   @return Returns a Job struct.
            */
            job parseVectorToJob(std::vector<std::string> message);

            /**
            *   @brief Continue a execution of a Job with JobPID
            *   @param jobPID Pid of the job to continue execution
            */
            void execute_job(pid_t jobPID);

            /**
            *   @brief Pause a execution of a Job with JobPID
            *   @param jobPID Pid of the job to pause execution
            */
            void pause_job(pid_t jobPID);

            /**
            *   @brief Kill a execution of a Job with JobPID
            *   @param jobPID Pid of the job to pause kill
            */
            void kill_job(pid_t jobPID);

            /**
            *   @brief RoundRobin Scheduler. Responsible for control when and which process execute or pause.
            */
            void round_robin();

            /**
            *   @brief Fork Server Process and Execute image of new process.
            *   @param job  Job that contains informations about new process.
            *   @return Returns the PID of created process
            */
            pid_t start_job(JobExecution job);

            /**
            *   @brief Verifies if a process that will execute, is alive or dead.
            *   @return Returns TRUE if process still alive of False if process is Dead.
            */
            bool refreshJobs();

            /**
            *   @brief Update time for execution of Jobs in list.
            *   @param spentTime Time Spent since last update.
            */
            void updateNewJobsTime(double spentTime);
};

#endif // ESCALONADOR_H_INCLUDED
