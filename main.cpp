#include <iostream>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "Escalonador.h"
#include "Estrutura.h"
#include "Executor.h"
#include "Execucao_Processo.h"


using namespace std;

/*
ExecucaoProcesso::ExecucaoProcesso(){

}
ExecucaoProcesso::~ExecucaoProcesso(){

}

Escalonador::Escalonador() {

}

Escalonador::~Escalonador() {

}
///setup
void Escalonador::setup(char** argv) {

}
///rcvMensagem
void Escalonador::rcvMensagem() {

}
///round roubin
void Escalonador::round_robin() {

}
///atualiza tempo
void Escalonador::atualizaTempoNovosProcessos(double tempoGasto) {

}
*/


Escalonador escalonador;

int main(int argc, char** argv)
{

    //Escalonador escalonador;
    //ExecucaoProcesso ultimoProcessoRecebido;

    //escalonador=new Escalonador();

    escalonador.setup(argv);
    std::cout << "Escalonador em espera." << std::endl;

    clock_t begin, end;


    double tempogasto;

    while(true) {
        begin = clock();

        escalonador.rcvMensagem();
        if(escalonador._processosEmExecucao.size()>0){
            escalonador.round_robin();

        }

        end = clock();

        tempogasto = (double)(end - begin) / CLOCKS_PER_SEC;
        escalonador.atualizaTempoNovosProcessos(tempogasto);

    }



    return 0;
}
