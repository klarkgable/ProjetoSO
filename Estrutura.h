#ifndef ESTRUTURA_H_INCLUDED
#define ESTRUTURA_H_INCLUDED

/// Error macros
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
#define MSQ_KEY2 0x159561
#define MSG_TYPE 1
#define SHM_KEY2 0x111142 /// Second Message Queue Key

/// Messages
#define MSG_CANCEL "Cancel" /// Message to cancel a job
#define MSG_LIST "List" /// Message to request the jobs list
#define MSG_SHUTDOWN "Shutdown" /// Message to shutdown the server

typedef struct job {
    short _jobId; /// Unique
    std::string _programName; /// Path
    int _hour;
    int _minute;
    int _times;
} Job;

typedef struct jobmsg {
	long _mtype;
	char _job[100];
} JobMessage;


#endif // ESTRUTURA_H_INCLUDED
