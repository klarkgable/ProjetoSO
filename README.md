# ProjetoSO
Projeto disponível em: https://github.com/klarkgable/ProjetoSO

Departamento de Ciência da Computação - Universidade de Brasíla 

Projeto para disciplina de sistemas operacionais 1°/2018

Título: Gerente de escalonamento postergado de processos CPU/bound 

Professora: Alba Cristina

Alunos:

      Klark Gable Souza Porto mat: 120015421
      
      Fábio Oliveira Guimarães mat: 180052659
      
Projeto implementando em linguagem C++ e compilado pelo compilador Gcc e testado em dois sistemas operacionais: Windows e Linux      
Nosso projeto foi organizado em módulos, e basicamente temos dois módulos principais:

      Módulo Executor - Responsável por implementar a chamada de execução dos  processos que serão escalonados pelo módulo Escalonador. O módulo Executor é composto pelo arquivo principal Executor.cpp e o arquivo Executor.h que é o cabeçalho definindo macros, classes e métodos para o modulo Executor. Esse módulo erencia a criação e adia a execução de novos processos. Recebe entrada do usuário e passa para o escalonador.

      Módulo Escalonador: Responsável por realizar em background o escalonamento de processos, seguindo a política round-roubin e fila de prioridades. O módulo Escalonador é composto pelo arquivo pricnipal Escalonador.cpp e o arquivo Escalonador.h que é o cabeçalho definindo macros, classes e métodos para o módulo escalonador. Essem módulo erencia o próprio processo de execução com a política Round Robin. Recebe entrada do módulo Executor, chama os programas nos momentos apropriados.

Além desses módulos, temos o arquivo Estrutura.h que vem definindo macros gerais para o projeto, e definindo a struct para processo e mensagem de processo. 

Cada módulo principal tem sua main que relaciona os módulos necessários e realiza a execução do projeto em si com as chamadas dos métodos.



Para executaqr o projeto, basta colocar todos os arquivos em uma pasta e compilar e chamar execução. Em shell, fazer as chamadas ao programa( solicita_execucao, remove_postergado, lista_postergados e Shutdown_postergado). O Escalonador estará rodando em background.


Mecanismos de IPC utilizados:
      
      Fila de mensagens: para enviar nós de processo e trocar mensagens relevantes entre os processos.
      
      Memória compartilhada: apenas uma variável - processoId, que permite ao Executor atribuir um número exclusivo a um processo recém-criado.
      
      Sinais e Alarmes: para executar programas no tempo adequado.



