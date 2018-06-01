#include <iostream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/types.h>
//#include <sys/ipc.h>
#include <sys/param.h>
//#include <sys/msg.h>
//#include <sys/shm.h>
#include <list>
#include <vector>
#include "Estrutura.h"
#include "Executor.h"
#include "Execucao_Processo.h"




/// Erros
#define SUCCESS 0
#define ERROR_ARGS -1
#define ERROR_FUNC -2
#define ERROR_SHM 1
#define ERROR_MSQ 2

/// IPCs
#define SHM_FLAGS 0644
#define SHM_KEY 0x015421
#define MSQ_FLAGS 0644
#define MSQ_KEY 0x052659
#define MSG_TYPE 1

/// Mensagens
#define MSG_CANCEL "Cancel"
#define MSG_REQLIST "List"

///Schedule Macros
#define QUANTUM = 5;



#ifndef ESCALONADOR_H_INCLUDED
#define ESCALONADOR_H_INCLUDED

/*class ExecucaoProcesso {

    private:

        pid_t _pid = -1;
        time_t recebeTempo;
        double tempoProximaExecucao;


    public:
        int copiasRestantes = _processo._copias;

        processo _processo;

        void setRecebeTempo();

        time_t getRecebeTempo();

        void setProcesso(processo rcvProcesso);

        Processo getProcesso();

        int getProcessoID();

        void setPID(pid_t pid);

        pid_t getPID();

        ExecucaoProcesso();

        void resetTempoProximaExecucao();

        void decreaseTempoProximaExecucao(double dTempo);

        double getTempoProximaExecucao();

        bool operator==(const ExecucaoProcesso &other) const;
};

*/

 ///Classe do escalonador
class Escalonador {
	private:
            ///atributos
            int _shmId;
            int _shmId2;
            int *_tamanhoLista;
            int *_processoId;
            int _msqId;
            int _msqId2;

	public:

        ///Metodos publicos

            std::list<ExecucaoProcesso> _novosProcessos; ///Recebe processos
            std::list<ExecucaoProcesso> _processosEmExecucao;
            ExecucaoProcesso _processoEmExecucao;

           ///construtor
            Escalonador();

    		~Escalonador();


    		void main(int argc, char** argv);

    		void setup(char** argv);

            ///recebe mensagem do modulo Executor
            void rcvMensagem();

           ///divide string em substrings
            std::vector<std::string> splitString(std::string input, std::string delimiter);

            ///analisa vetor de mensagem de processo e retorna uma struct do processo
            processo analisaVetorProcesso(std::vector<std::string> mensagem);

            ///continua execuçao de processo com pid dele
            void executa_processo(pid_t processoPID);

            ///pausa execucao de processo com pid
            void pausa_processo(pid_t processoPID);

            ///mata execucao de proceeso pid
            void kill_processo(pid_t processoPID);

           ///ira fazer o controle de processos por escalonamento round robin
            void round_robin();

            ///inicia novo processo e retorna pid dele
            pid_t inicia_processo(ExecucaoProcesso processo);

           ///verifica se o processo que vai se executar está ativo ou morto e retorna true ou false
            bool verificaProcessos();

            ///atualiza tempo para execucao dos processos
            void atualizaTempoNovosProcessos(double tempoGasto);

            void shutdown();
};

#endif // ESCALONADOR_H_INCLUDED
