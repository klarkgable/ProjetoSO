#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <algorithm>
#include "Estrutura.h"
#include "Escalonador.h"


JobExecution::JobExecution(){

}
void JobExecution::setJob(job rcvJob) {
    this->_job = rcvJob;
}

void JobExecution::setPID(pid_t pid) {
    this->_pid = pid;
}

int JobExecution::getJobID() {
    return this->_job._jobId;
}

Job JobExecution::getJob() {
    return this->_job;
}

pid_t JobExecution::getPID(){
    return this->_pid;
}

void JobExecution::setReceivedTime() {
    time( &(this->receivedTime) );
}

time_t JobExecution::getReceivedTime(){
    return this->receivedTime;
}

bool JobExecution::operator==(const JobExecution &other) const {
   return this->_job._jobId == other._job._jobId;
}

void JobExecution::resetTimeToNextExecution() {
    this->timeToNextExecution = this->getJob()._hour*3600 + this->getJob()._minute*60;
}

void JobExecution::decreaseTimeToNextExecution(double dTime) {
    this->timeToNextExecution -=dTime;
}

double JobExecution::getTimeToNextExecution() {
    return this->timeToNextExecution;
}


using namespace std;

Server::Server() {

}

Server::~Server() {

}

void Server::setup(char** argv) {
	// Memoria compartilhada
	if((this->_shmId = shmget(SHM_KEY, sizeof(int), SHM_FLAGS | IPC_CREAT)) == -1) { // Cria
		std::cout << "Erro: Falha no shmget." << std::endl;
		exit(1);
	}

	this->_jobId = (int *) shmat(this->_shmId, NULL, 0);
	if(errno  == -1) { // Attachment
		std::cout << "Erro: Falha no shmat." << std::endl;
		exit(1);
	}
	this->_jobId[0] = 0; /// Formato Array

    //Memoria compartilhada para o tamanho 
    if((this->_shmId2 = shmget(SHM_KEY2, sizeof(int), SHM_FLAGS | IPC_CREAT)) == -1) { // Cria
        std::cout << "Erro:  Falha no shmget." << std::endl;
        exit(1);
    }

    this->_listLenght = (int *) shmat(this->_shmId2, NULL, 0);
    if(errno  == -1) { // Attachment
        std::cout << "Erro: Falha no shmat." << std::endl;
        exit(1);
    }
    this->_listLenght[0] = 0; /// Formato Array

	// Fila de Mensagens
	if((this->_msqId = msgget(MSQ_KEY, MSQ_FLAGS | IPC_CREAT)) == -1) { // Cria a fila de mensagens
		std::cout << "Erro: Falha no msgget." << std::endl;
		exit(2);
	}

    //Fila de mensagem para impressao
    // Fila de mensagem
    if((this->_msqId2 = msgget(MSQ_KEY2, MSQ_FLAGS | IPC_CREAT)) == -1) { // Cria a fila de menssagem
        std::cout << "Erro: Falha no msgget." << std::endl;
        exit(2);
    }
}

void Server::shutdown() {
	std::cout << "Encerrando Executor - Shutting down." << std::endl;

	// Segmento de memoria compartilhada
	if(shmdt(this->_jobId) == -1) { // Detach
		std::cout << "Erro: Falha no shmdt." << std::endl;
		exit(1);
	}
	if(shmctl(this->_shmId, IPC_RMID, NULL) == -1) { // Remove
		std::cout << "Erro: Falha no shmctl." << std::endl;
		exit(1);
	}

	// Fila de menssagem
	if(msgctl(this->_msqId, IPC_RMID, NULL) == -1) {
		std::cout << "Erro: Falha no msgctl." << std::endl;
		exit(2);
	}

    // Segmento de memoria compartilhada
    if(shmdt(this->_listLenght) == -1) { // Detach
        std::cout << "Erro: Falha no shmdt." << std::endl;
        exit(1);
    }
    if(shmctl(this->_shmId2, IPC_RMID, NULL) == -1) { // Remove
        std::cout << "Erro: Falha no shmctl." << std::endl;
        exit(1);
    }

    // Fila de menssagem
    if(msgctl(this->_msqId2, IPC_RMID, NULL) == -1) {
        std::cout << "Erro: Falha no msgctl." << std::endl;
        exit(2);
    }

    std::list<JobExecution>::iterator it = _jobsInExecution.begin();
    while(it!=_jobsInExecution.end()){
        this->kill_job(it->getPID());
        std::next(it,1);
    }
}

void Server::rcvMessage() {
	size_t msgsize;
	jobmsg msgp;
    JobExecution nullJob;
//    this->shutdown();
	int result = msgrcv(this->_msqId, &msgp, msgsize, MSG_TYPE, IPC_NOWAIT);

    if(result != (ssize_t)-1) {
        JobExecution rcvJob;
        std::string var = msgp._job;
        vector<string> jobvalues = splitString(var, "|");
        if (jobvalues.size() >= 5) {
            rcvJob.setJob(parseVectorToJob(jobvalues));
            rcvJob.resetTimeToNextExecution();
            rcvJob.remaingTimes = rcvJob.getJob()._times;
            this->_jobId[0] = (rcvJob.getJobID())+1;
            _newJobs.insert(_newJobs.end(), rcvJob);
            std::cout << "Processo recebido:" + var << endl;
            rcvJob.setReceivedTime();
        }else if(jobvalues.operator[](1) == MSG_CANCEL) {
            job iteratorJob;
            JobExecution job;
            iteratorJob._jobId = std::stoi(jobvalues.operator[](0), nullptr, 10);
            job.setJob(iteratorJob);
            _newJobs.remove(job);
            std::cout << "Processo removido com ID: "+std::to_string(job.getJobID())<<endl;
        }else if(jobvalues.operator[](1) == MSG_SHUTDOWN){
            this->shutdown();
            exit(0);
        }else{
            this->_listLenght[0] = _newJobs.size();
            std::list<JobExecution>::iterator it = _newJobs.begin();
            while(it!= _newJobs.end()){
                std::string messsageToSend = std::to_string(it->getJob()._jobId)+"|"+
                        std::to_string(it->getJob()._hour)+"|"+std::to_string(it->getJob()._minute)+"|"+
                        std::to_string(it->remaingTimes)+"|"+it->getJob()._programName;
                JobMessage msg;
                msg._mtype = 1;
                strcpy(msg._job,messsageToSend.c_str());
                if(msgsnd(_msqId2,&msg,sizeof(JobMessage),MSQ_FLAGS|IPC_NOWAIT) == -1){
                    std::cout << "Erro: Erro no msgsnd."<<endl;
                    return;
                }
                std::next(it,1);
            }
        }
	}

}

void Server::execute_job(pid_t jobPID) {
    kill(jobPID,SIGCONT);
}

void Server::pause_job(pid_t jobPID) {
    kill(jobPID,SIGSTOP);
}

void Server::kill_job(pid_t jobPID){
    kill(jobPID,SIGKILL);
}

void Server::round_robin() {

    std::list<JobExecution>::iterator it = _jobsInExecution.begin();
    cout <<"Agendador iniciado para o trabalho ID "+std::to_string(it->getJobID())<<std::endl;
    _jobInExecution = (*it);
    if(this->refreshJobs() == false){
        _jobsInExecution.remove((*it));
        std::cout << "Processo com PID: "+std::to_string(_jobInExecution.getPID())+" Terminando a execucao"<<endl;
        std::cout << "Processos esperando a execucao: "+std::to_string(_jobsInExecution.size())<<endl;
        _jobInExecution.remaingTimes -= 1;
        if(_jobInExecution.remaingTimes > 0){
            std::cout << "Processo com PID: "+std::to_string(_jobInExecution.getPID())+" Volta a lista de espera" << endl;
            std::cout << "Processo com PID: "+std::to_string(_jobInExecution.getPID())+
                                 " Tempo restante: "+
                                 std::to_string(_jobInExecution.remaingTimes) << endl;
        }
        return;
    }
    if(it->getPID() == -1){
        _jobInExecution.setPID(start_job((*it)));

    }else{
        execute_job(_jobInExecution.getPID());
    }

    cout<<"Comecou a execucao do processo com o PID: "+std::to_string(_jobInExecution.getPID())<<endl;

    sleep(10);

    this->pause_job(_jobInExecution.getPID());
    _jobsInExecution.pop_front();
    _jobsInExecution.push_back(_jobInExecution);
    cout<<"Pausa da execucao do processo com o PID:"+std::to_string(_jobInExecution.getPID())<<endl;
}

pid_t Server::start_job(JobExecution job) {
    pid_t pid = fork();
    if(pid==0){
        int status = execl(job.getJob()._programName.c_str(),job.getJob()._programName.c_str(),NULL);
        std::cout <<"Endereco: "+ job.getJob()._programName<<endl;
        std::cout <<"Flag do Processo: "+ std::to_string(status)<<endl;
        std::cout <<"Flag do ERRNO: "+ std::to_string(errno)<<endl;

        if(status == -1){
            cout << "Erro na criacao do Processo" <<endl;
            exit(-2);
        }
    }

    return pid;
}

bool Server::refreshJobs(){
    int status;

    pid_t result = waitpid(_jobInExecution.getPID(), &status, WNOHANG);

    if (result == 0) {
        return true;
    } else if (result == -1) {
        return -1;
    } else {
        cout<<"Processo com PID:"+std::to_string(result)+"esta morto"<<endl;
        return false;
    }
}

vector<string> Server::splitString(string input, string delimiter) {
    vector<string> output;
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

job Server::parseVectorToJob(vector<string> message){
    job job;
    job._jobId = std::stoi(std::string(message.operator[](0).c_str()), nullptr,10);
    job._hour = std::stoi(std::string(message.operator[](1).c_str()), nullptr,10);
    job._minute = std::stoi(std::string(message.operator[](2).c_str()), nullptr,10);
    job._times = std::stoi(std::string(message.operator[](3).c_str()), nullptr,10);
    job._programName = std::string(message.operator[](4).c_str());
    return job;
}

void Server::updateNewJobsTime(double spentTime){
    if(_newJobs.size()>0) {
        int iteratorsize = _newJobs.size();
        std::list<JobExecution>::iterator it = this->_newJobs.begin();
        while (iteratorsize >0) {
            it->decreaseTimeToNextExecution(spentTime);
            if (it->getTimeToNextExecution() <= 0) {
                std::cout << "Processo com PID: " + std::to_string(it->getJob()._jobId) + " Entra em execucao" <<
                std::endl;
                JobExecution job = (*it);
                this->_newJobs.remove((*it));
                job.remaingTimes-=1;
                this->_jobsInExecution.push_back(job);
                if(it->remaingTimes > 0) {
                    job.resetTimeToNextExecution();
                    this->_newJobs.insert(_newJobs.end(),job);
                }
                if(this->_newJobs.size()<=0){
                    return;
                }
            }
            it = std::next(it, 1);
            iteratorsize--;
        }
        this->_listLenght[0] = _newJobs.size();
    }
}
