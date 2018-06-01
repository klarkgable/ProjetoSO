#include <cstring>
#include <string>
#include <iostream>

#ifndef ESTRUTURA_H_INCLUDED
#define ESTRUTURA_H_INCLUDED

/// Erros
#define SUCCESS 0
#define ERROR_ARGS -1
#define ERROR_FUNC -2
#define ERROR_SHM 1
#define ERROR_MSQ 2

/// IPCs
#define SHM_FLAGS 0644 /// flag de memoria
#define SHM_KEY 0x015421 /// chave de memoria
#define MSQ_FLAGS 0644 /// Flag de mensagem
#define MSQ_KEY 0x052659 /// chave de mensagem
#define MSQ_KEY2 0x052660
#define MSG_TYPE 1
#define SHM_KEY2 0x015422

/// Mensagens
#define MSG_CANCEL "Cancel" /// Mensagem para cancelar um processo
#define MSG_LIST "List" /// Mensagem para requerer lista de processos
#define MSG_SHUTDOWN "Shutdown" /// Mensagem para shutdown


///estrutura para processo
typedef struct processo {
    short _processoId; ///identificador unico para tupla
    std::string _nomePrograma;
    int _hora;
    int _minutos;
    int _copias;
    int _pri; ///prioridade do processo(1>2>3...)

} Processo;


///estrutura para troca de mensagem entre processos
typedef struct msgProcesso {
	long _mtipo;
	char _processo[100];
} MsgProcesso;


#endif // ESTRUTURA_H_INCLUDED
