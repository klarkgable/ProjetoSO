#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <algorithm>
#include "Estrutura.h"
#include "Escalonador.h"
#include "Executor.h"
#include "Execucao_Processo.h"

using namespace std;

Executor::Executor() {

}

Executor::~Executor() {

}

int main(int argc, char** argv) {
	Executor executor;

	switch(argc) {
		case 1:
		case 2:
        	case 3:
        	case 4:
		case 5:      ///5 argumentos(hora, minutos,copias,nome do arquivo e prioridade)
			executor.setup(argv);
			break;
		default:
			std::cout << "Numero de argumentos incompatível, conferir chamada do programa" << std::endl;
			return ERROR_ARGS;
	}

	executor.shutdown();
	return SUCCESS;
}


///funcionamento dos metodos declarados em Executor.h
void Executor::setup(char** argv) {
	std::string funcionalidade = argv[0];


	if((this->_shmId = shmget(SHM_KEY, 0, 0)) == -1) { ///criação
		std::cout << "Erro: shmget falhou." << std::endl;
		exit(1);
	}
	(this->_processoId = (int *) shmat(this->_shmId, NULL, 0));

	if(errno  == -1) {
		std::cout << "Erro: shmat falhou." << std::endl;
		exit(1);
	}

	///Tamanho da Lista
	if((this->_shmId2 = shmget(SHM_KEY2, 0, 0)) == -1) { /// Criaçao
		std::cout << "Erro: shmget falhou." << std::endl;
		exit(1);
	}

	(this->_tamanhoLista = (int *) shmat(this->_shmId2, NULL, 0));

	if(errno  == -1) {
		std::cout << "Erro: shmat falhou." << std::endl;
		exit(1);
	}

	/// Filas de mensagens
	if((this->_msqId = msgget(MSQ_KEY, MSQ_FLAGS)) == -1) {
		std::cout << "Erro: msgget falhou." << std::endl;
		exit(2);
	}

	if((this->_msqId2 = msgget(MSQ_KEY2, MSQ_FLAGS)) == -1) {
		std::cout << "Erro: msgget falhou." << std::endl;
		exit(2);
	}

	/// descobrir qual operação iremos fazer
	std::string funcionalidadeExecucao = funcionalidade.substr(funcionalidade.length() - strlen("solicita_execucao"));
	std::string funcionalidadeRemover = funcionalidade.substr(funcionalidade.length() - strlen("remove_postergado"));
	std::string funcionalidadeListaProcesso = funcionalidade.substr(funcionalidade.length() - strlen("lista_postergados"));
	std::string funcionalidadeShutdown = funcionalidade.substr(funcionalidade.length() - strlen("Shutdown_postergado"));

	if(!funcionalidadeExecucao.compare("solicita_execucao")) {
		short horas, minutos;
		int pri;
		pri = 1;

		this->analisaTempo(argv[1], &horas, &minutos);

		this->executor(horas, minutos, atoi(argv[2]), argv[3], pri);
	}
	else if(!funcionalidadeRemover.compare("remove_postergado")) {
		this->naoExecuta(atoi(argv[1]));
	}
	else if(!funcionalidadeListaProcesso.compare("lista_postergados")) {
		this->listaProcessos();
	}
	else if(!funcionalidadeShutdown.compare("Shutdown_postergado")) {
		this->shutdownEscalonador();
	}
	else {
		std::cout << "Funcionalidade desconhecida: " << argv[0] << std::endl;
		exit(ERROR_FUNC);
	}
}

///funcionalidade de solicitar execucao
void Executor::executor(short horas = 0, short minutos = 0, int copias = 1, std::string path = "", int pri = 1) {
    MsgProcesso processoCorrente;

    this->_processoId[0]++;

	std::string mensagem = std::to_string(this->_processoId[0]) + "|" +
						  std::to_string(horas) + "|" +
						  std::to_string(minutos) + "|" +
						  std::to_string(copias) + "|" + path +
						  std::to_string(pri);

    processoCorrente._mtipo = 1;
	strcpy(processoCorrente._processo , mensagem.c_str());
    this->enviaMensagem(processoCorrente);

    std::cout << "Novo processo criado." << std::endl;
    std::cout << "Id\t" << "Delay\t" << "Copias\t" << "Programa\t" << "Prioridade\t" << std::endl;
    std::cout << this->_processoId[0] + 1 << "\t" << horas << ":" << minutos << "\t"<< copias << "\t" << path << "\t" << pri << std::endl;
}

///funcionalidade de remover execucao
void Executor::naoExecuta(int id) {
	MsgProcesso controlaMensagem;

	controlaMensagem._mtipo = 1;

	std::string mensagem = std::to_string(id) + "|" +
						  MSG_CANCEL;
	strcpy(controlaMensagem._processo, mensagem.c_str());

	this->enviaMensagem(controlaMensagem);
}

///funcionalidade de listar processos
void Executor::listaProcessos() {
	MsgProcesso controlaMensagem;

	controlaMensagem._mtipo = 1;

	std::string mensagem = std::to_string(0) + "|" +
						  MSG_LIST;

	strcpy(controlaMensagem._processo, mensagem.c_str());

	this->enviaMensagem(controlaMensagem);

	if(this->_tamanhoLista[0] > 0) {
		int i = this->_tamanhoLista[0];
		std::cout << "Id\t" << "Delay\t" << "Copias\t" << "Program" << "Prioridade" << std::endl;
		while(i > 1) {
			/// Recebe mensagem
			size_t msgtamanho;
			msgtamanho = 10;
			MsgProcesso no;
			msgrcv(this->_msqId2, &no, msgtamanho, MSG_TYPE, 0);
			std::vector<std::string> vetorTupla = this->splitString(no._processo, "|");
			std::cout << vetorTupla.operator[](0).c_str() << "\t" << vetorTupla.operator[](1).c_str()
						<< ":" << vetorTupla.operator[](2).c_str() << "\t" << vetorTupla.operator[](3).c_str()
						<< "\t" << vetorTupla.operator[](4).c_str() << std::endl;
			i--;
		}
	}
	else {
		std::cout << "Não há processos em execucao." << std::endl;
	}
}

void Executor::shutdownEscalonador() {
	MsgProcesso controlaMensagem;

	controlaMensagem._mtipo = 1;

	std::string mensagem = std::to_string(0) + "|" +
						  MSG_SHUTDOWN;

	strcpy(controlaMensagem._processo, mensagem.c_str());

	this->enviaMensagem(controlaMensagem);
}

void Executor::shutdown() {
	if(shmdt(this->_processoId) == -1) {
		std::cout << "Erro: shmdt falhou." << std::endl;
		exit(1);
	}
	if(shmdt(this->_tamanhoLista) == -1) {
		std::cout << "Erro: shmdt falhou." << std::endl;
		exit(1);
	}
}

void Executor::analisaTempo(char *stringTempo, short *horas, short *minutos) {
	char *analisador;

	analisador = strtok(stringTempo, ":");
	*horas = atoi(analisador);
	analisador = strtok(NULL, ":");
	*minutos = atoi(analisador);
	///ignora o que vem depois
}

void Executor::enviaMensagem(MsgProcesso mensagem) {

	if(msgsnd(this->_msqId, &mensagem, sizeof(MsgProcesso), MSQ_FLAGS | IPC_NOWAIT) == -1) {
		std::cout << "Erro: msgsnd falhou." << std::endl;
		exit(2);
	}
}

std::vector<std::string> Executor::splitString(std::string input, std::string delimiter) {
    std::vector<std::string> output;
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
