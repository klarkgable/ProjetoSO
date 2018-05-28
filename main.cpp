#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "Escalonador.h"
#include "Estrutura.h"



using namespace std;

Server server;
JobExecution JobExecution;

int main(int argc, char** argv) {

    JobExecution lastReceivedJob;
    server.setup(argv);
    std::cout << "Server on Standby." << std::endl;
    clock_t begin, end;


    double timespent;

    while(true) {
        begin = clock();
        server.rcvMessage();
        if(server._jobsInExecution.size()>0){
            server.round_robin();
        }
        end = clock();
        timespent = (double)(end - begin) / CLOCKS_PER_SEC;
        server.updateNewJobsTime(timespent);
    }
    // TODO: capture "ctrl+c" from terminal and shut down whatever is needed.
    // What is needed: shut down shared memory segment and message queue.
//    server.shutdown();
//
//    return SUCCESS;
}
