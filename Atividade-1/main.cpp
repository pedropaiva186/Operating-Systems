#include "include/auxiliares.h"

int main()
{
    // Estrutura que armazenará os comandos que serão executados no terminal
    std::vector<std::string> commands;

    // Inserindo os quatros comandos necessários
    commands.push_back("ls");
    commands.push_back("date");
    commands.push_back("pwd");
    commands.push_back("whoami");

    // Executando a função com os comandos
    filho_principal(commands);
}