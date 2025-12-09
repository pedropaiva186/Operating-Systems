#include "../include/auxiliares.h"

void filho_principal(std::vector<std::string> & commands_all)
{
    std::vector<pid_t> pids; // Vector que armazenará os processos filhos criados

    for(int i = 0; i < 2; i++)
    {
        pid_t procs = fork(); // Criando os processos filhos

        if(procs == 0) // Caso seja o processo filho
        {
            if(i == 1) // Caso seja F1
            {
                std::vector<std::string> first_part(commands_all.begin(), commands_all.begin() + 2);
                filho_secundario(first_part);

                // Printando o PID do processo
                std::cout << "PID F1: " << getpid() << "\n";
                std::cout << "PID Pai do F1: " << getppid() << "\n";
            }
            else // F2
            {
                std::vector<std::string> second_part(commands_all.begin() + 2, commands_all.end());
                filho_secundario(second_part);

                // Printando o PID do processo
                std::cout << "PID F2: " << getpid() << "\n";
                std::cout << "PID Pai do F2: " << getppid() << "\n";
            }

            _exit(0); // Excluindo o processo filho
        }
        else // Será o processo pai
        {
            // Salvandos os processos num vetor, para utilizar eles depois
            pids.push_back(procs);
        }
    }

    for(pid_t pid: pids) // Esperando todos os filhos terminarem
    {
        int status;
        waitpid(pid, &status, 0);
    }

    std::cout << "Sou o processo pai P1\n";
    std::cout << "PID: " << getpid() << "\n";
    std::cout << "Encerrando programa.\n";
}

void filho_secundario(std::vector<std::string> & commands)
{
    std::vector<pid_t> pids; // Estrutura que armazenará os ids do processos para esperar todos os filhos acabarem antes do pai

    for(auto & com : commands)
    {
        pid_t procs = fork(); // Cria um novo processo para cada comando

        if(procs == 0) // Caso seja processo filho
        {
            // Passando o comando em string para char*
            char* args[] = {
                const_cast<char*>(com.c_str()),
                nullptr
            };

            execvp(args[0], args); // Executando os comandos que foram passados

            /*
            * Optamos por usar "execvp" pois ele permite uma melhor escolha dos comandos que vamos executar, dando a liberdade do
            * próprio usuário escolher os comandos que serão executados
            */

            perror("Execução dos comandos falhou!"); // Caso a exec tenha dado errado
            _exit(1);
        }
        else // Será o processo pai
        {
            pids.push_back(procs);
        }
    }

    for(pid_t pid: pids) // Esperando todos os filhos terminarem
    {
        int status;
        waitpid(pid, &status, 0);
    }
}