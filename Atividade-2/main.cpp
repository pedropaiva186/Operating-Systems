#include "include/auxiliares.h"

int main()
{
    // Criando o vector dentro das restrições da questão
    std::vector<int> vector_int = gerar_vector(10000, 0, 100);

    // Executando os comandos usando multithread
    exec_process_multithread(vector_int);

    // Executand o processo de forma comum, com uma única thread
    exec_process_unithread(vector_int);
}