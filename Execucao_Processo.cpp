#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <algorithm>
#include "Estrutura.h"
#include "Escalonador.h"
#include "Executor.h"
#include "Execucao_Processo.h"


using namespace std;

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
    time( &(this->recebeTempo) );
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
