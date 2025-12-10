#include "../include/auxiliares.h"

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

void exec_process()
{
    // Coletando o tempo de início do algoritmo
    auto inicio = std::chrono::steady_clock::now();

    // Criando o vector dentro das restrições da questão
    std::vector<int> vector_int = gerar_vector(10000, 0, 100);

    // Criando uma thread para cada função, onde faço as chamadas dessa maneira para poder coletar os retornos das funções
    auto f_media = std::async(std::launch::async, calc_media, std::ref(vector_int));
    auto f_mediana = std::async(std::launch::async, calc_mediana, vector_int);
    auto f_dp      = std::async(std::launch::async, calc_desvio_padrao, std::ref(vector_int));

    // Coletando os valores de retorno das funções
    float media = f_media.get();
    float mediana = f_mediana.get();
    float dp = f_dp.get();

    // Coletando o tempo após as operações do algoritmoi
    auto fim = std::chrono::steady_clock::now();

    // Fazendo o cálculo da duração, e deixando-o no formato de exibição
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);

    // Exibindo os valores de saída
    std::cout << "Media: " << media << "\n";
    std::cout << "Mediana: " << mediana << "\n";
    std::cout << "Desvio padrão: " << dp << "\n";
    std::cout << "Tempo de execução: " << duracao.count() << "ms\n";
}