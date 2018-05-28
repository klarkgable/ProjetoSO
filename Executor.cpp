#include "../header/scheduler.h"

Scheduler::Scheduler() {

}

Scheduler::~Scheduler() {

}

int main(int argc, char** argv) {
	Scheduler scheduler;

	switch(argc) {
		case 1: // Totally flawless workaround solution --Needs refactoring
		case 2:
		case 4:
			scheduler.setup(argv);
			break;
		default:
			std::cout << "Incompatible number of arguments. Check program call." << std::endl;
			return ERROR_ARGS;
	}

	scheduler.shutdown();
	return SUCCESS;
}

void Scheduler::setup(char** argv) {
	std::string functionality = argv[0];

	// Shared Memory - Job ID
	if((this->_shmId = shmget(SHM_KEY, 0, 0)) == -1) { // Creation
		std::cout << "Error: shmget failed." << std::endl;
		exit(1);
	}
	(this->_jobId = (int *) shmat(this->_shmId, NULL, 0));

	if(errno  == -1) { // Attachment
		std::cout << "Error: shmat failed." << std::endl;
		exit(1);
	}

	// Shared Memory - List Length
	if((this->_shmId2 = shmget(SHM_KEY2, 0, 0)) == -1) { // Creation
		std::cout << "Error: shmget failed." << std::endl;
		exit(1);
	}

	(this->_listLength = (int *) shmat(this->_shmId2, NULL, 0));

	if(errno  == -1) { // Attachment
		std::cout << "Error: shmat failed." << std::endl;
		exit(1);
	}

	// Message Queues
	if((this->_msqId = msgget(MSQ_KEY, MSQ_FLAGS)) == -1) {
		std::cout << "Error: msgget failed." << std::endl;
		exit(2);
	}

	if((this->_msqId2 = msgget(MSQ_KEY2, MSQ_FLAGS)) == -1) {
		std::cout << "Error: msgget failed." << std::endl;
		exit(2);
	}

	// Figuring out what to do
	std::string functionalityPpExec = functionality.substr(functionality.length() - strlen("executa_postergado"));
	std::string functionalityPpRemv = functionality.substr(functionality.length() - strlen("remove_postergado"));
	std::string functionalityListJb = functionality.substr(functionality.length() - strlen("lista_postergados"));
	std::string functionalityServerShutdown = functionality.substr(functionality.length() - strlen("shutdown_postergado"));

	if(!functionalityPpExec.compare("executa_postergado")) {
		short hours, minutes;
		// Parsing time string to integers
		this->parseTime(argv[1], &hours, &minutes);
		// Scheduling the process
		this->schedule(hours, minutes, atoi(argv[2]), argv[3]);
	}
	else if(!functionalityPpRemv.compare("remove_postergado")) {
		this->unschedule(atoi(argv[1]));
	}
	else if(!functionalityListJb.compare("lista_postergados")) {
		this->listJobs();
	}
	else if(!functionalityServerShutdown.compare("shutdown_postergado")) {
		this->shutdownServer();
	}
	else {
		std::cout << "Unknown functionality: " << argv[0] << std::endl;
		exit(ERROR_FUNC);
	}
}

void Scheduler::schedule(short hours = 0, short minutes = 0, int times = 1, std::string path = "") {
    JobMessage currentJob;

    this->_jobId[0]++;

	std::string message = std::to_string(this->_jobId[0]) + "|" +
						  std::to_string(hours) + "|" +
						  std::to_string(minutes) + "|" +
						  std::to_string(times) + "|" + path;

    currentJob._mtype = 1;
	strcpy(currentJob._job , message.c_str());
    this->sendMessage(currentJob);

    std::cout << "New job created." << std::endl;
    std::cout << "Id\t" << "Delay\t" << "Times\t" << "Program" << std::endl;
    std::cout << this->_jobId[0] + 1 << "\t" << hours << ":" << minutes << "\t"<< times << "\t" << path << std::endl;
}

void Scheduler::unschedule(int id) {
	JobMessage controlMessage;

	controlMessage._mtype = 1;

	std::string message = std::to_string(id) + "|" +
						  MSG_CANCEL;
	strcpy(controlMessage._job, message.c_str());

	this->sendMessage(controlMessage);
}

void Scheduler::listJobs() {
	JobMessage controlMessage;

	controlMessage._mtype = 1;

	std::string message = std::to_string(0) + "|" +
						  MSG_LIST;

	strcpy(controlMessage._job, message.c_str());

	this->sendMessage(controlMessage);

	if(this->_listLength[0] > 0) {
		int i = this->_listLength[0];
		std::cout << "Id\t" << "Delay\t" << "Times\t" << "Program" << std::endl;
		while(i > 1) {
			// Receive message
			size_t msgsize;
			JobMessage node;
			msgrcv(this->_msqId2, &node, msgsize, MSG_TYPE, 0);
			std::vector<std::string> tupleVector = this->splitString(node._job, "|");
			std::cout << tupleVector.operator[](0).c_str() << "\t" << tupleVector.operator[](1).c_str()
						<< ":" << tupleVector.operator[](2).c_str() << "\t" << tupleVector.operator[](3).c_str()
						<< "\t" << tupleVector.operator[](4).c_str() << std::endl;
			i--;
		}
	}
	else {
		std::cout << "No jobs scheduled." << std::endl;
	}
}

void Scheduler::shutdownServer() {
	JobMessage controlMessage;

	controlMessage._mtype = 1;

	std::string message = std::to_string(0) + "|" +
						  MSG_SHUTDOWN;

	strcpy(controlMessage._job, message.c_str());

	this->sendMessage(controlMessage);
}

void Scheduler::shutdown() {
	if(shmdt(this->_jobId) == -1) {
		std::cout << "Error: shmdt failed." << std::endl;
		exit(1);
	}
	if(shmdt(this->_listLength) == -1) {
		std::cout << "Error: shmdt failed." << std::endl;
		exit(1);
	}
}

void Scheduler::parseTime(char *timeString, short *hours, short *minutes) {
	char *parser;
	parser = strtok(timeString, ":");
	*hours = atoi(parser); // Parses hours.
	parser = strtok(NULL, ":");
	*minutes = atoi(parser); // Parses minutes.
	// What ever comes after is ignored.
}

void Scheduler::sendMessage(JobMessage message) {
	if(msgsnd(this->_msqId, &message, sizeof(JobMessage), MSQ_FLAGS | IPC_NOWAIT) == -1) {
		std::cout << "Error: msgsnd failed." << std::endl;
		exit(2);
	}
}

std::vector<std::string> Scheduler::splitString(std::string input, std::string delimiter) {
    std::vector<std::string> output;
    char *pch;
    char *str = strdup(input.c_str());
    pch = strtok (str, delimiter.c_str());

    while (pch != NULL)	{
        output.push_back(pch);
        pch = strtok (NULL,  delimiter.c_str());
    }

    free(str);
    return output;
}
