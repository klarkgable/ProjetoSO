# ProjetoSO
Projeto disponível em: https://github.com/klarkgable/ProjetoSO

Departamento de Ciência da Computação - Universidade de Brasíla 

Projeto para disciplina de sistemas operacionais 1°/2018

Título: Gerente de escalonamento postergado de processos CPU/bound 

Professora: Alba Cristina

Alunos:

      Klark Gable Souza Porto mat: 120015421
      
      Fábio Oliveira Guimarães mat: 180052659
      
Projeto implementando em linguagem C++ e compilado pelo compilador Gcc e testado em dois sistemas operacionais: Windows(problemas com diferenças de inlcude de algumas bibliotecas que só aceitam em sistemas unix) e Linux. 

Nosso projeto foi organizado em módulos, e basicamente temos dois módulos principais:

      Módulo Executor - Responsável por implementar a chamada de execução dos  processos que serão escalonados pelo módulo Escalonador. O módulo Executor é composto pelo arquivo principal Executor.cpp e o arquivo Executor.h que é o cabeçalho definindo macros, classes e métodos para o modulo Executor. Esse módulo gerencia a criação e adia a execução de novos processos. Recebe entrada do usuário e passa para o escalonador.

      Módulo Escalonador: Responsável por realizar em background o escalonamento de processos, seguindo a política round-roubin e fila de prioridades. O módulo Escalonador é composto pelo arquivo pricnipal Escalonador.cpp e o arquivo Escalonador.h que é o cabeçalho definindo macros, classes e métodos para o módulo escalonador. Essem módulo gerencia o próprio processo de execução com a política Round Robin. Recebe entrada do módulo Executor, chama os programas nos momentos apropriados.

Temos o módulo auxiliar para execução dos processos, que  vem dividido nos arquivos Execucao_Processo.cpp e Execeucao_Processo.h e formas base para o módulo escalonador.

Além desses módulos, temos o arquivo Estrutura.h que vem definindo macros gerais para o projeto, e definindo a struct para processo e mensagem de processo e por fim o arquivo main.cpp que é como se fosse a main do móculo escalonador, faz chamadas aos métodos do escalonador que aparece após a chamada de programa via shell pelo usuário.

Obs: No módulo executor ele conta com um método main também para chamar execução do executor.




Para rodar o projeto, criamos um arquivo makefile com as diretrizes de compilação então basta abrir terminal e rodar o make e utilizar em shell os comandos de teste, mas cabe a quem for compilar a opção de compilar diretamente com os arquivos em uma pasta, mas seguindo os passos abaixo para funcionar:

      Basta colocar todos os arquivos em uma pasta e compilar e chamar execução. Nesse caso irá rodar o arquivo main.cpp que equivale ao escalonador que irá ficar em espera em background. Após isso, compilar em nova pasta contendo os arquivos Executor.cpp, Executor.h e Estrutura.h pra rodar o excecutor enquanto escalonador trabalha em background. fazer as chamadas ao programa( solicita_execucao, remove_postergado, lista_postergados e Shutdown_postergado).



Mecanismos de IPC utilizados:
      
      Fila de mensagens: para enviar nós de processo e trocar mensagens relevantes entre os processos.
      
      Memória compartilhada: apenas uma variável - processoId, que permite ao Executor atribuir um número exclusivo a um processo recém-criado.
      
      Sinais e Alarmes: para executar programas no tempo adequado.



