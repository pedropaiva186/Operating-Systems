#include "../include/auxiliares.h"

float g_media;
float g_mediana;
float g_dp;

std::vector<int> gerar_vector(int n, int lb, int ub)
{
    // Configurando a seed para os números aleatórios que serão gerados
    std::random_device rd;
    std::mt19937 gen(rd());

    // Definindo o gerador dentro do intervalo
    std::uniform_int_distribution<int> dist(lb, ub);

    // Criando o vetor que será retornado
    std::vector<int> v(n);

    // Inserindo os elementos aleatórios
    for(int i = 0; i < n; i++)
    {
        v[i] = dist(gen);
    }

    return v;
}

float calc_media(std::vector<int> & v)
{
    int sum = 0;

    for(int i = 0; i < v.size(); i++)
    {
        sum += v[i];
    }

    return sum / (v.size() * 1.0);
}

float calc_mediana(std::vector<int> v)
{
    // Ordenando o vetor
    std::sort(v.begin(), v.end());

    int center = v.size() / 2;
    float mediana = 0;

    if(v.size() % 2 == 0) // Caso seja par, executamos o cálculo da média dos valores centrais
    {
        mediana = (v[center] + v[center-1]) / 2;
    }
    else mediana = v[center];

    return mediana;
}

float calc_desvio_padrao(std::vector<int> & v)
{
    double sum = 0;
    float m = calc_media(v); // Refazendo o cálculo da média para que uma thread não dependa de outra para executar

    // Somando todos os valores, subtraindo a média
    for(int i = 0; i < v.size(); i++)
    {
        sum += pow(v[i] - m, 2);
    }

    double dp = sum / v.size();

    dp = sqrt(dp);

    return dp;
}

void exec_process_multithread(std::vector<int> & vector_int)
{
    // Coletando o tempo de início do algoritmo
    auto inicio = std::chrono::steady_clock::now();

    // Criando uma thread para cada função, onde faço as chamadas dessa maneira para poder coletar os retornos das funções
    auto f_media = std::async(std::launch::async, calc_media, std::ref(vector_int));
    auto f_mediana = std::async(std::launch::async, calc_mediana, vector_int);
    auto f_dp = std::async(std::launch::async, calc_desvio_padrao, std::ref(vector_int));

    // Coletando apenas o tempo de criação das threads
    auto criacao_threads = std::chrono::steady_clock::now();

    // Coletando os valores de retorno das funções
    g_media = f_media.get();
    g_mediana = f_mediana.get();
    g_dp = f_dp.get();

    // Coletando o tempo após as operações do algoritmoi
    auto fim = std::chrono::steady_clock::now();

    // Cálculo de criação das threads
    auto duracao_threads = std::chrono::duration_cast<std::chrono::microseconds>(criacao_threads - inicio);

    // Fazendo o cálculo da duração, e deixando-o no formato de exibição
    auto duracao_total = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);

    // Exibindo os valores de saída
    std::cout << "Media: " << g_media << "\n";
    std::cout << "Mediana: " << g_mediana << "\n";
    std::cout << "Desvio padrão: " << g_dp << "\n";
    std::cout << "Tempo de execução total: " << duracao_total.count() << "us\n";
    std::cout << "Tempo de criação das threads: " << duracao_threads.count() << "us\n";
}

void exec_process_unithread(std::vector<int> & vector_int)
{
    // Coletando o tempo de início do algoritmo
    auto inicio = std::chrono::steady_clock::now();

    // Executando as funções
    g_media = calc_media(vector_int);
    g_mediana = calc_mediana(vector_int);
    g_dp = calc_desvio_padrao(vector_int);

    // Coletando o tempo após as operações do algoritmoi
    auto fim = std::chrono::steady_clock::now();

    // Fazendo o cálculo da duração, e deixando-o no formato de exibição
    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);

    // Exibindo os valores de saída
    std::cout << "Media: " << g_media << "\n";
    std::cout << "Mediana: " << g_mediana << "\n";
    std::cout << "Desvio padrão: " << g_dp << "\n";
    std::cout << "Tempo de execução: " << duracao.count() << "us\n";
}

void exec_process_multiprocess(std::vector<int> & v)
{
    // Criando um pipeline de leitura e escrita para cada processo
    int pipe_media[2], pipe_mediana[2], pipe_dp[2];
    pipe(pipe_media);
    pipe(pipe_mediana);
    pipe(pipe_dp);

    // Coletando o tempo inicial
    auto inicio = std::chrono::steady_clock::now();

    // Criando um processo para cada operação
    if (fork() == 0)
    {
        close(pipe_media[0]); // Fechando o pipeline para começar a leitura
        float media = calc_media(v); // Fazendo o cálculo escolhido
        write(pipe_media[1], &media, sizeof(float)); // Salvando a informação no pipeline
        close(pipe_media[1]); // Fechando o pipeline de escrita
        _exit(0); // Deletando o processo que foi criado
    }

    if (fork() == 0)
    {
        close(pipe_mediana[0]);
        float mediana = calc_mediana(v);
        write(pipe_mediana[1], &mediana, sizeof(float));
        close(pipe_mediana[1]);
        _exit(0);
    }

    if (fork() == 0)
    {
        close(pipe_dp[0]);
        float dp = calc_desvio_padrao(v);
        write(pipe_dp[1], &dp, sizeof(float));
        close(pipe_dp[1]);
        _exit(0);
    }

    // Caso chegue aqui, estamos lidando com o processo pai

    // Fechando os pipelines de escrita para fazer a leitura
    close(pipe_media[1]);
    close(pipe_mediana[1]);
    close(pipe_dp[1]);

    // Lendo as informações
    read(pipe_media[0], &g_media, sizeof(float));
    read(pipe_mediana[0], &g_mediana, sizeof(float));
    read(pipe_dp[0], &g_dp, sizeof(float));

    // Fechando os pipelines de leitura
    close(pipe_media[0]);
    close(pipe_mediana[0]);
    close(pipe_dp[0]);

    // Espera todos os filhos
    wait(nullptr);
    wait(nullptr);
    wait(nullptr);

    // Fazendo os cálculos dos tempos
    auto fim = std::chrono::steady_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);

    // Exibindo as saídas
    std::cout << "Media: " << g_media << "\n";
    std::cout << "Mediana: " << g_mediana << "\n";
    std::cout << "Desvio padrao: " << g_dp << "\n";
    std::cout << "Tempo (processos): " << duracao.count() << "us\n";
}
