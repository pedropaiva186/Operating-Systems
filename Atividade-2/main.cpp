#include "include/auxiliares.h"

int main()
{
    // Criando o vector dentro das restrições da questão
    std::vector<int> vector_int = gerar_vector(10000, 0, 100);

    std::cout << "Execução com uma única Thread:" << "\n";
    // Executando o processo de forma comum, com uma única thread
    exec_process_unithread(vector_int);

    /*
    * Consideramos a execução com uma única thread a mesma com um único processo porque na prática eles funcionam da mesma maneira, por isso, para
    * simplificar, faço a exibição deles apenas uma vez
    */

    std::cout << "\n";

    std::cout << "Execução Multithread:" << "\n";
    // Executando os comandos usando multithread
    exec_process_multithread(vector_int);

    std::cout << "\n";

    std::cout << "Execução Três Processos:" << "\n";
    // Executando multiprocessos
    exec_process_multiprocess(vector_int);
}