#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "Escalonador.h"
#include "Estrutura.h"
#include "Executor.h"


using namespace std;

Escalonador escalonador;


int main(int argc, char** argv) {

    //Escalonador escalonador;
    //ExecucaoProcesso ultimoProcessoRecebido;
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
    // TODO: capture "ctrl+c" from terminal and shut down whatever is needed.
    // What is needed: shut down shared memory segment and message queue.
//    server.shutdown();
//
   //return SUCCESS;
}
