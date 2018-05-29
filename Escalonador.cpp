#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <algorithm>
#include "Estrutura.h"
#include "Escalonador.h"


ExecucaoProcesso::ExecucaoProcesso(){

}

///recebe processo
void ExecucaoProcesso::setProcesso(processo rcvProcesso) {
    this->_processo = rcvProcesso;
}

///setar pid
void ExecucaoProcesso::setPID(pid_t pid) {
    this->_pid = pid;
}

int ExecucaoProcesso::getProcessoID() {
    return this->_processo._processoId;
}

Processo ExecucaoProcesso::getProcesso() {
    return this->_processo;
}

///retornar pid
pid_t ExecucaoProcesso::getPID(){
    return this->_pid;
}

void ExecucaoProcesso::setRecebeTempo() {
    tempo( &(this->recebeTempo) );
}

time_t ExecucaoProcesso::getRecebeTempo(){
    return this->recebeTempo;
}

///executa processo identificado pelo id de tupla
bool ExecucaoProcesso::operator==(const ExecucaoProcesso &other) const {
   return this->_processo._processoId == other._processo._processoId;
}

///resetar tempo para proxima execucao
void ExecucaoProcesso::resetTempoProximaExecucao() {
    this->tempoProximaExecucao = this->getProcesso()._hora*3600 + this->getProcesso()._minutos*60; ///tempo estimado para procima execuçao em segundos
}

///decrementa tempo
void ExecucaoProcesso::decreaseTempoProximaExecucao(double dTempo) {
    this->tempoProximaExecucao -=dTempo;
}

///retorna tempo para proxima execucao
double ExecucaoProcesso::getTempoProximaExecucao() {
    return this->tempoProximaExecucao;
}


using namespace std;

Escalonador::Escalonador() {

}

Escalonador::~Escalonador() {

}

void Escalonador::setup(char** argv) {
	///implemntando segmento de memoria compartilhada
    if((this->_shmId = shmget(SHM_KEY, sizeof(int), SHM_FLAGS | IPC_CREAT)) == -1) { ///criaçao
		std::cout << "Erro: shmget falhou." << std::endl;
		exit(1);
	}

	this->_processoId = (int *) shmat(this->_shmId, NULL, 0);
	if(errno  == -1) { // Attachment
		std::cout << "Erro: shmat falhou." << std::endl;
		exit(1);
	}
	this->_processoId[0] = 0;

    ///memoria compartilhada para o tamnho da lista de processos
    if((this->_shmId2 = shmget(SHM_KEY2, sizeof(int), SHM_FLAGS | IPC_CREAT)) == -1) { ///criaçao
        std::cout << "Error: shmget falhou." << std::endl;
        exit(1);
    }

    this->_tamanhoLista = (int *) shmat(this->_shmId2, NULL, 0);
    if(errno  == -1) { // Attachment
        std::cout << "Erro: shmat falhou." << std::endl;
        exit(1);
    }
    this->_tamanhoLista[0] = 0;

	/// fila de mensagens
	if((this->_msqId = msgget(MSQ_KEY, MSQ_FLAGS | IPC_CREAT)) == -1) { /// Criaçao da fila
		std::cout << "Erro: msgget falhou." << std::endl;
		exit(2);
	}

   ///enviar lista de processos para impressao
    if((this->_msqId2 = msgget(MSQ_KEY2, MSQ_FLAGS | IPC_CREAT)) == -1) { ///criaçao da fila
        std::cout << "Erro: msgget falhou." << std::endl;
        exit(2);
    }
}

void Escalonador::shutdown() {
	std::cout << "Desligando." << std::endl;

	///segmento de memoria compartilhada
	if(shmdt(this->_processoId) == -1) { ///desanexando
		std::cout << "Erro: shmdt falhou." << std::endl;
		exit(1);
	}
	if(shmctl(this->_shmId, IPC_RMID, NULL) == -1) { /// Removendo
		std::cout << "Erro: shmctl falhou." << std::endl;
		exit(1);
	}

	///fila de mensagens
	if(msgctl(this->_msqId, IPC_RMID, NULL) == -1) {
		std::cout << "Erro: msgctl falhou." << std::endl;
		exit(2);
	}

    ///segmento de memoria compartilhada
    if(shmdt(this->_tamanhoLista) == -1) { ///desanexando
        std::cout << "Erro: shmdt falhou." << std::endl;
        exit(1);
    }
    if(shmctl(this->_shmId2, IPC_RMID, NULL) == -1) { ///Removendo
        std::cout << "Erro: shmctl falhou." << std::endl;
        exit(1);
    }

    ///fila de mensagens
    if(msgctl(this->_msqId2, IPC_RMID, NULL) == -1) {
        std::cout << "Erro: msgctl falhou." << std::endl;
        exit(2);
    }

    std::list<ExecucaoProcesso>::iterator it = _processosEmExecucao.begin();
    while(it!=_processosEmExecucao.end()){
        this->kill_processo(it->getPID());
        std::next(it,1);
    }
}

void Escalonador::rcvMensagem() {
	size_t msgtamanho;
	msgProcesso msgp;
    ExecucaoProcesso processoNull;


	int resultado= msgrcv(this->_msqId, &msgp, msgtamanho, MSG_TYPE, IPC_NOWAIT);

    if(resultado != (ssize_t)-1) {
        ExecucaoProcesso rcvProcesso;
        std::string var = msgp._processo;
        vector<string> processoValores = splitString(var, "|");

        if (processoValores.size() >= 5) {
            rcvProcesso.setProcesso(analisaVetorProcesso(processoValores));
            rcvProcesso.resetTempoProximaExecucao();
            rcvProcessso.copiasRestantes = rcvProcesso.getProcesso()._copias;
            this->_processoId[0] = (rcvProcesso.getProcessoID())+1;
            _novosProcessos.insert(_novosProcessos.end(), rcvProcesso);
            std::cout << "Processo recebido:" + var << endl;
            rcvProcesso.setRecebeTempo();

        }else if(processoValores.operator[](1) == MSG_CANCEL) {
            processo iteradorProcesso;
            ExecucaoProcesso processo;
            iteradorProcesso._processoId = std::stoi(processoValores.operator[](0), nullptr, 10);
            processo.setProcesso(iteradorProcesso);
            _novosProcessos.remove(processo);
            std::cout << "Remove Processo com o id: "+std::to_string(processo.getProcessoID())<<endl;
        }else if(processoValores.operator[](1) == MSG_SHUTDOWN){
            this->shutdown();
            exit(0);
        }else{
            this->_tamanhoLista[0] = _novosProcessos.size();
            std::list<ExecucaoProcesso>::iterator it = _novosProcessos.begin();
            while(it!= _novosProcessos.end()){
                std::string mensagemParaEnvio = std::to_string(it->getProcesso()._processoId)+"|"+
                        std::to_string(it->getProcesso()._horas)+"|"+std::to_string(it->getProcesso()._minutos)+"|"+
                        std::to_string(it->copiasRestantes)+"|"+it->getProcesso()._nomePrograma;

                MsgProcesso msg;
                msg._mtipo = 1;
                strcpy(msg._processo,mensagemParaEnvio.c_str());
                if(msgsnd(_msqId2,&msg,sizeof(MsgProcesso),MSQ_FLAGS|IPC_NOWAIT) == -1){
                    std::cout << "Erro: msgsnd deu erro"<<endl;
                    return;
                }
                std::next(it,1);
            }
        }
	}

}

        ///sinais usados pelo escalonador

void Escalonador::executa_processo(pid_t processoPID) {
    kill(proceesoPID,SIGCONT);
}

void Escalonador::pausa_processo(pid_t processoPID) {
    kill(processoPID,SIGSTOP);
}

void Escalonador::kill_processo(pid_t processoPID){
    kill(processoPID,SIGKILL);
}

void Escalonador::round_robin() {

    std::list<ExecucaoProcesso>::iterator it = _processosEmExecucao.begin();
    cout <<"Executor iniciou pelo Processo com ID: "+std::to_string(it->getProcessoID())<<std::endl;
    _processoEmExecucao = (*it);
    if(this->verificaProcessos() == false){
        _processosEmExecucao.remove((*it));
        std::cout << "Processo com PID: "+std::to_string(_processoEmExecucao.getPID())+" Terminou execucao."<<endl;
        std::cout << "Processos restantes para Execucao: "+std::to_string(_processosEmExecucao.size())<<endl;
        _processoEmExecucao.copiasRestantes -= 1;
        if(_processoEmExecucao.copiasRestantes > 0){
            std::cout << "Processo com PID: "+std::to_string(_processoEmExecucao.getPID())+" Volta para lista de espera" << endl;
            std::cout << "Processo com PID: "+std::to_string(_processoEmExecucao.getPID())+
                                 " Copias restantes: "+
                                 std::to_string(_processoEmExecucao.copiasRestantes) << endl;
        }
        return;
    }
    if(it->getPID() == -1){
        _processoEmExecucao.setPID(inicia_processo((*it)));

    }else{
        executa_processo(_processoEmExecucao.getPID());
    }

    cout<<"Iniciado a Execucao de Processo com PID: "+std::to_string(_processoEmExecucao.getPID())<<endl;

    sleep(10);

    this->pausa_processo(_processoEmExecucao.getPID());
    _processosEmExecucao.pop_front();
    _processosEmExecucao.push_back(_processoEmExecucao);
    cout<<"Pausa Execucao de Processo com PID:"+std::to_string(_processoEmExecucao.getPID())<<endl;
}


pid_t Escalonador::inicia_processo(ExecucaoProcesso processo) {
    pid_t pid = fork();
    if(pid==0){
        int status = execl(processo.getProcesso()._nomePrograma.c_str(),processo.getProcesso()._nomePrograma.c_str(),NULL);
        std::cout <<"Caminho URL: "+ processo.getProcesso()._nomePrograma<<endl;
        std::cout <<"Flag do processo: "+ std::to_string(status)<<endl;
        std::cout <<"Flag de ERRNO: "+ std::to_string(errno)<<endl;

        if(status == -1){
            cout << "Erro na criacao dos processos" <<endl;
            exit(-2);
        }
    }

    return pid;
}

bool Escalonador::verificaProcessos(){
    int status;

    pid_t resultado = waitpid(_processoEmExecucao.getPID(), &status, WNOHANG);

    if (resultado == 0) {
        return true;
    } else if (resultado == -1) {
        return -1;
    } else {
        cout<<"Processo com PID:"+std::to_string(resultado)+"esta MORTO"<<endl;
        return false;
    }
}

vector<string> Escalonador::splitString(string input, string delimiter) {
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

processo Escalonador::analisaVetorProcesso(vector<string> mensagem){

    processo processo;
    processo._processoId = std::stoi(std::string(mensagem.operator[](0).c_str()), nullptr,10);
    processo._horas = std::stoi(std::string(mensagem.operator[](1).c_str()), nullptr,10);
    processo._minutos = std::stoi(std::string(mensagem.operator[](2).c_str()), nullptr,10);
    processo._copias = std::stoi(std::string(mensagem.operator[](3).c_str()), nullptr,10);
    processo._nomePrograma = std::string(mensagem.operator[](4).c_str());
    ///processo._pri = std::stoi(std::string(mensagem.operator[](5).c_str()), nullptr,10);

    return processo;
}

void Escalonador::atualizaTempoNovosProcessos(double tempoGasto){
    if(_novosProcessos.size()>0) {
        int iteratorTamanho = _novosProcessos.size();
        std::list<ExecucaoProcesso>::iterator it = this->_novosProcessos.begin();
        while (iteratorTamanho >0) {
            it->decreaseTempoProximaExecucao(tempoGasto);
            if (it->getTempoProximaExecucao() <= 0) {
                std::cout << "Processo com ID: " + std::to_string(it->getProcesso()._processoId) + "Vai para execucao " <<
                std::endl;
                ExecucaoProcesso processo = (*it);
                this->_novosProcessos.remove((*it));
                processo.copiasRestantes-=1;
                this->_processosEmExecucao.push_back(processo);
                if(it->copiasRestantes > 0) {
                    processo.resetTempoProximaExecucao();
                    this->_novosProcessos.insert(_novosProcessos.end(),processo);
                }
                if(this->_novosProcessos.size()<=0){
                    return;
                }
            }
            it = std::next(it, 1);
            iteratorTamanho--;
        }
        this->_tamanhoLista[0] = _novosProcessos.size();
    }
}
