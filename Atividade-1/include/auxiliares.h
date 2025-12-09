#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>

/*
* Função responsável por gerar F1 e F2, ela terá o papel de criar um processo filho, e ao fim de sua execução, exibir no terminal
* um texto apresentando que o processo terminou e seu PID.
* Params:
* => commands_all: Lista de comandos que serão passados para os filhos secundários executarem no terminal linux
*/
void filho_principal(std::vector<std::string> & commands_all);

/*
* Função responsável por gerar um filho e executar no terminal um comando que lhe foi passado como parâmetro.
* Params:
* => command: Comandos que serão executados no terminal unix pelos processos filho
*/
void filho_secundario(std::vector<std::string> & command);