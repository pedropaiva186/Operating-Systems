#include <vector>
#include <thread>
#include <random>
#include <algorithm>
#include <cmath>
#include <future>
#include <chrono>
#include <iostream>

std::vector<int> gerar_vector(int n, int lb, int ub);

float calc_media(std::vector<int> & v);

float calc_mediana(std::vector<int> v);

float calc_desvio_padrao(std::vector<int> & v);

void exec_process_multithread(std::vector<int> & v);

void exec_process_unithread(std::vector<int> & v);