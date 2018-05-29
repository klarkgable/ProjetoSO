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
#define SHM_KEY 0x111141 /// chave de memoria
#define MSQ_FLAGS 0644 /// Flag de mensagem
#define MSQ_KEY 0x159560 /// chave de mensagem
#define MSQ_KEY2 0x159561
#define MSG_TYPE 1
#define SHM_KEY2 0x111142

/// Mensagens
#define MSG_CANCEL "Cancel" /// Mensagem para cancelar um processo
#define MSG_LIST "List" /// Mensagem para requerer lista de processos
#define MSG_SHUTDOWN "Shutdown" /// Mensagem para shutdown

typedef struct processo {
    short _processoId; ///identificador unico para tupla
    std::string _nomePrograma;
    int _hora;
    int _minutos;
    int _copias;
    int pri; ///prioridade do processo(1>2>3...)

} Processo;


typedef struct msgProcesso {
	long _mtipo;
	char _processo[100];
} MsgProcesso;


#endif // ESTRUTURA_H_INCLUDED
