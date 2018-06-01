#include <iostream>
#include <cstring>
#include <string>
#include "Estrutura.h"


#ifndef EXECUCAO_PROCESSO_H_INCLUDED
#define EXECUCAO_PROCESSO_H_INCLUDED



class ExecucaoProcesso {

    private:

        ///atributos
        pid_t _pid = -1;
        time_t recebeTempo;
        double tempoProximaExecucao;


    public:

        ///Metodos

        ExecucaoProcesso();

        ~ExecucaoProcesso();


        int copiasRestantes = _processo._copias;

        processo _processo;

        void setRecebeTempo();

        time_t getRecebeTempo();

        void setProcesso(processo rcvProcesso);

        Processo getProcesso();

        int getProcessoID();

        void setPID(pid_t pid);

        pid_t getPID();

        void resetTempoProximaExecucao();

        void decreaseTempoProximaExecucao(double dTempo);

        double getTempoProximaExecucao();

        bool operator==(const ExecucaoProcesso &other) const;
};



#endif // EXECUCAO_PROCESSO_H_INCLUDED
