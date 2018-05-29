#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <sys/types.h>
//#include <sys/ipc.h>
#include <sys/param.h>
//#include <sys/msg.h>
//#include <sys/shm.h>
#include "Estrutura.h"



#ifndef EXECUTOR_H_INCLUDED
#define EXECUTOR_H_INCLUDED

class Executor {
        private:

                int *_processoId;
                int *_tamanhoLista;
                int _msqId;
                int _msqId2;
                int _shmId;
                int _shmId2;



               ///analisa uma string de tempo
                void analisaTempo(char *tempoString, short *hora, short *minutos);

                ///envia uma mensagem
                void enviaMensagem(MsgProcesso mensagem);

                ///divide string em substring
                std::vector<std::string> splitString(std::string input, std::string delimiter);

	public:

            Executor();


        	~Executor();


        	void main(int argc, char** argv);

        	void setup(char** argv);


            ///funçao de solicitar execuçao
        	void executor(short horas, short minutos, int copias, std::string nomePorgrama,int pri);

        	void naoExecuta(int id);

            void listaProcessos();

            void shutdownEscalonador();

            void shutdown();
};


#endif // EXECUTOR_H_INCLUDED
