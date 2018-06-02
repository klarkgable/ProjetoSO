# ProjetoSO
Projeto disponível em: https://github.com/klarkgable/ProjetoSO

Departamento de Ciência da Computação - Universidade de Brasíla 

Projeto para disciplina de sistemas operacionais 1°/2018

Título: Gerente de escalonamento postergado de processos CPU/bound 

Professora: Alba Cristina

Alunos:

      Klark Gable Souza Porto mat: 120015421
      
      Fábio Oliveira Guimarães mat: 180052659
      
Projeto implementando em linguagem C++ e compilado pelo compilador Gcc e testado em dois sistemas operacionais:No UNIX(ambientes linux) que é o foco da disciplina e Windows(problemas com diferenças de inlcude de algumas bibliotecas que só aceitam em sistemas unix, mas consegue rodar também apesar de usar algumas funções de unix). 

Nosso projeto foi organizado em módulos, e basicamente temos dois módulos principais:

      Módulo Executor - Responsável por implementar a chamada de execução dos  processos que serão escalonados pelo módulo Escalonador. O módulo Executor é composto pelo arquivo principal Executor.cpp e o arquivo Executor.h que é o cabeçalho definindo macros, classes e métodos para o modulo Executor. Esse módulo gerencia a criação e adia a execução de novos processos. Recebe entrada do usuário e passa para o escalonador.

      Módulo Escalonador: Responsável por realizar em background o escalonamento de processos, seguindo a política round-roubin e fila de prioridades. O módulo Escalonador é composto pelo arquivo pricnipal Escalonador.cpp e o arquivo Escalonador.h que é o cabeçalho definindo macros, classes e métodos para o módulo escalonador. Essem módulo gerencia o próprio processo de execução com a política Round Robin. Recebe entrada do módulo Executor, chama os programas nos momentos apropriados.

Temos o módulo auxiliar para execução dos processos, que  vem dividido nos arquivos Execucao_Processo.cpp e Execeucao_Processo.h. Esses arquivos são base para o módulo escalonador.

Além desses módulos, temos o arquivo Estrutura.h que vem definindo macros gerais para o projeto, e definindo a struct para processo e mensagem de processo e por fim o arquivo main.cpp que é como se fosse a main do móculo escalonador, faz chamadas aos métodos do escalonador que roda em espera(background).

Obs: No módulo executor ele conta com um método main também para chamar execução do executor.




Para rodar o projeto, criamos um arquivo makefile com as diretrizes de compilação. Então basta baixar arquivos do projeto para uma pasta, abrir terminal e caminhar até a pasta do projeto(comando cd) e rodar o make(para isso, seu sistema tem que ta instalado os compiladores, o g++ e o make). Estando na pasta do projeto, digite no terminal o comando "make -f makefile.txt" que ele ira gerar os .o e executáveis para rodar o projeto. Após criar os "arquivos .o" e executáveis(Escalonador, Solicita_execucao, remove_postergado, lista_postergados e Shutdown_postergado) já é possível começar a execução em dois passos:

      1) Em terminal digite ./Escalonador para rodar o escalonador. Aparecerá a mensagem "Escalonador em espera". Então abra novo terminal e caminhe novamente para a pasta do projeto. Obs:Caso tenha terminado de usar o escalonador, pode-se apertar ctrl+c no terminal para poder digitar qualquer comando ou entrar em outra pasta.
      
      2) No novo terminal digite a funcionalidade pretendida(Solicita_execucao[parâmetros], remove_postergado, lista_postergados ou Shutdown_postergado) com o comando ./funcionalidade
            exemplo: ./solicita_execucao[parametros]
                     ./lista_postergados
                     ...
          
                     

 Caso optar por compilar sem o arquivo makefile, terá que ir compilando separadamente os arquivos, pois todos compoem o projeto, mas Executor e Escalonador cada um tem seus arquivos .cpp e .h de base.
 
 



Mecanismos de IPC utilizados:
      
      Fila de mensagens: para enviar nós de processo e trocar mensagens relevantes entre os processos.
      
      Memória compartilhada: apenas uma variável - processoId, que permite ao Executor atribuir um número exclusivo a um processo recém-criado.
      
      Sinais e Alarmes: para executar programas no tempo adequado.



