#include <stdio.h>
#include <stdlib.h> // srand, rand
#include <time.h>   // time
#include <stdbool.h>
#include "utils.c"

#define INT_MAX 2147483647

int TAM_MAX_ARRAY = 50;
int num_comparacoes = 0;

// MARK: INSERTION SORT

void insertionSort(int arr[], int pos_inicio, int N)
{

    // Starting from the second element
    num_comparacoes++;
    for (int i = pos_inicio + 1; i < N; i++)
    {
        int key = arr[i];
        int j = i - 1;

        // Move elements of arr[0..i-1], that are
        // greater than key, to one position to
        // the right of their current position
        num_comparacoes++;
        while (j >= pos_inicio && arr[j] > key)
        {
            num_comparacoes++;
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        // Move the key to its correct position
        arr[j + 1] = key;
    }
}

// MARK: SWAP

// função que realiza a troca entre dois elementos
void troca(int vet[], int i, int j)
{
    int aux = vet[i];
    vet[i] = vet[j];
    vet[j] = aux;
}

// MARK: PARTICIONA ARRAY

// particiona e retorna o índice do pivô
int particiona(int vet[], int inicio, int fim)
{
    int pivo, pivo_indice, i;

    pivo = vet[fim]; // o pivô é sempre o último elemento
    pivo_indice = inicio;

    num_comparacoes++;
    for (i = inicio; i < fim; i++)
    {
        // verifica se o elemento é <= ao pivô
        num_comparacoes++;
        if (vet[i] <= pivo)
        {
            // realiza a troca
            troca(vet, i, pivo_indice);
            // incrementa o pivo_indice
            pivo_indice++;
        }
    }

    // troca o pivô
    troca(vet, pivo_indice, fim);

    // retorna o índice do pivô
    return pivo_indice;
}

// escolhe um pivô aleatório para evitar o pior caso do quicksort
int particiona_random(int vet[], int inicio, int fim)
{
    // seleciona um número entre fim (inclusive) e inicio (inclusive)
    int medio = (inicio + fim) / 2;
    int pivo_indice = 0;

    if (vet[inicio] <= vet[medio] && vet[inicio] >= vet[fim] || vet[inicio] >= vet[medio] && vet[inicio] <= vet[fim])
    {
        pivo_indice = inicio;
    }
    else if (vet[medio] <= vet[inicio] && vet[medio] >= vet[fim] || vet[medio] >= vet[inicio] && vet[medio] <= vet[fim])
    {
        pivo_indice = medio;
    }
    else
    {
        pivo_indice = fim;
    }

    // faz a troca para colocar o pivô no fim
    troca(vet, pivo_indice, fim);
    // chama a particiona
    return particiona(vet, inicio, fim);
}

// MARK: QUICK SORT

void quick_sort(int vet[], int inicio, int fim, bool hibrid, bool improved)
{
    num_comparacoes++;
    if (hibrid && fim - inicio <= TAM_MAX_ARRAY)
    {
        num_comparacoes--;
        // printf("Usando Insertion Sort\n");
        insertionSort(vet, inicio, fim + 1);
    }
    else if (inicio < fim)
    {
        // função particionar retorna o índice do pivô
        int pivo_indice = improved ? particiona_random(vet, inicio, fim) : particiona(vet, inicio, fim);

        // chamadas recursivas quick_sort
        quick_sort(vet, inicio, pivo_indice - 1, hibrid, improved);
        quick_sort(vet, pivo_indice + 1, fim, hibrid, improved);
    }
    num_comparacoes++;
}

// MARK: OPEN FILE 

int *open_file_and_read_array(const char *filename, int size)
{
    FILE *file = fopen(filename, "r");
    int *array = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        fscanf(file, "%d", &array[i]);
    }
    fclose(file);
    return array;
}

// MARK: ARRAY UTILS

int *clone_array(int *vet, int n) {
    int *copy = (int*)malloc(n * sizeof(int));
    memcpy(copy, vet, n * sizeof(int));
    return copy;
}

// MARK: EMPIRIC COMPARISONS

int *get_best_tam_array(int vet[], int tam_vet, bool hibrid, bool improved)
{
    int best_tam = 0;
    int lowest_com = INT_MAX;
    int *best_results = (int *)malloc(2 * sizeof(int));

    for (int j = 1; j <= 400; j++)
    {
        num_comparacoes = 0;
        TAM_MAX_ARRAY = j;

        // Cria copia do array para interaçao
        int *copy = clone_array(vet, tam_vet);

        // chamada do quicksort
        quick_sort(copy, 0, tam_vet - 1, hibrid, improved);

        // Libera memoria alocada
        free(copy);

        // printf("Tamanho do sub-vetor: %d \t-\t Numero de comparacoes: %d\n", TAM_MAX_ARRAY, num_comparacoes);
        if (num_comparacoes < lowest_com)
        {
            lowest_com = num_comparacoes;
            best_tam = TAM_MAX_ARRAY;
        }
    }
    best_results[0] = best_tam;
    best_results[1] = lowest_com;

    return best_results;
}

int get_average_result(int vet[], int tam_vet, int max_size_subArray, int num_executions, bool hibrid, bool improved)
{
    TAM_MAX_ARRAY = max_size_subArray;

    for (int i = 0; i < num_executions; i++)
    {
        num_comparacoes = 0;

        // Cria copia do array para interaçao
        int *copy = clone_array(vet, tam_vet);

        // chamada do quicksort
        quick_sort(copy, 0, tam_vet - 1, hibrid, improved);

        // Libera memoria alocada
        free(copy);
    }

    return num_comparacoes / num_executions;
}

// MARK: MAIN

int main()
{
    clock_t inicio, fim;
    double tempo_gasto;

    generate_tests_mass(10000);

    int TAM_ARRAY = 0;
    FILE *file;

    file = fopen("tests/size_test_mass.txt", "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(file, "%d", &TAM_ARRAY);

    fclose(file);

    int *random_vet = open_file_and_read_array("tests/random_test_mass.txt", TAM_ARRAY);
    int *repeated_vet = open_file_and_read_array("tests/repeated_test_mass.txt", TAM_ARRAY);
    int *reversed_vet = open_file_and_read_array("tests/reversed_test_mass.txt", TAM_ARRAY);
    int *sorted_vet = open_file_and_read_array("tests/sorted_test_mass.txt", TAM_ARRAY);
    int *worst_case_vet = open_file_and_read_array("tests/worst_case_test_mass.txt", TAM_ARRAY);

    // printf("random_vet: %d\n", random_vet[0]);

    // MARK: Question A

    printf("\n--- A - Resultados QuickSort Recursivo ---\n\n");

    inicio = clock();
    int a_lowest_comp = get_average_result(random_vet, TAM_ARRAY, TAM_MAX_ARRAY, 100, false, false);
    fim = clock();
    int a_lowest_com = a_lowest_comp;
    printf("Menor numero de comparacoes para o vetor aleatorio: %d\n", a_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    inicio = clock();
    int a_lowest_comp_repeated = get_average_result(repeated_vet, TAM_ARRAY, TAM_MAX_ARRAY, 100, false, false);
    fim = clock();
    a_lowest_com = a_lowest_comp_repeated;
    printf("Menor numero de comparacoes para o vetor repetido: %d\n", a_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    inicio = clock();
    int a_lowest_comp_reversed = get_average_result(reversed_vet, TAM_ARRAY, TAM_MAX_ARRAY, 100, false, false);
    fim = clock();
    a_lowest_com = a_lowest_comp_reversed;
    printf("Menor numero de comparacoes para o vetor reverso: %d\n", a_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    inicio = clock();
    int a_lowest_comp_sorted = get_average_result(sorted_vet, TAM_ARRAY, TAM_MAX_ARRAY, 100, false, false);
    fim = clock();
    a_lowest_com = a_lowest_comp_sorted;
    printf("Menor numero de comparacoes para o vetor ordenado: %d\n", a_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);
    
    inicio = clock();
    int a_lowest_comp_worst_case = get_average_result(worst_case_vet, TAM_ARRAY, TAM_MAX_ARRAY, 100, false, false);
    fim = clock();
    a_lowest_com = a_lowest_comp_worst_case;
    printf("Menor numero de comparacoes para o vetor pior caso: %d\n", a_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    // MARK: Question B

    printf("\n--- B - Resultados QuickSort Recursivo Hibrido ---\n");

    int *b_best_results = get_best_tam_array(random_vet, TAM_ARRAY, true, false);
    int b_best_tam = b_best_results[0];
    printf("\nMelhor tamanho de sub-vetor aleatorio: %d\n", b_best_tam);
    inicio = clock();
    int b_lowest_comp = get_average_result(random_vet, TAM_ARRAY, b_best_tam, 100, true, false);
    fim = clock();
    int b_lowest_com = b_lowest_comp;
    printf("Menor numero de comparacoes para o vetor aleatorio: %d\n", b_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *b_best_results_repeated = get_best_tam_array(repeated_vet, TAM_ARRAY, true, false);
    b_best_tam = b_best_results_repeated[0];
    printf("Melhor tamanho de sub-vetor repetido: %d\n", b_best_tam);
    inicio = clock();
    int b_lowest_comp_repeated = get_average_result(repeated_vet, TAM_ARRAY, b_best_tam, 100, true, false);
    fim = clock();
    b_lowest_com = b_lowest_comp_repeated;
    printf("Menor numero de comparacoes para o vetor repetido: %d\n", b_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *b_best_results_reversed = get_best_tam_array(reversed_vet, TAM_ARRAY, true, false);
    b_best_tam = b_best_results_reversed[0];
    printf("Melhor tamanho de sub-vetor reverso: %d\n", b_best_tam);
    inicio = clock();
    int b_lowest_comp_reversed = get_average_result(reversed_vet, TAM_ARRAY, b_best_tam, 100, true, false);
    fim = clock();
    b_lowest_com = b_lowest_comp_reversed;
    printf("Menor numero de comparacoes para o vetor reverso: %d\n", b_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *b_best_results_sorted = get_best_tam_array(sorted_vet, TAM_ARRAY, true, false);
    b_best_tam = b_best_results_sorted[0];
    printf("Melhor tamanho de sub-vetor ordenado: %d\n", b_best_tam);
    inicio = clock();
    int b_lowest_comp_sorted = get_average_result(sorted_vet, TAM_ARRAY, b_best_tam, 100, true, false);
    fim = clock();
    b_lowest_com = b_lowest_comp_sorted;
    printf("Menor numero de comparacoes para o vetor ordenado: %d\n", b_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *b_best_results_worst_case = get_best_tam_array(worst_case_vet, TAM_ARRAY, true, false);
    b_best_tam = b_best_results_worst_case[0];
    printf("Melhor tamanho de sub-vetor pior caso: %d\n", b_best_tam);
    inicio = clock();
    int b_lowest_comp_worst_case = get_average_result(worst_case_vet, TAM_ARRAY, b_best_tam, 100, true, false);
    fim = clock();
    b_lowest_com = b_lowest_comp_worst_case;
    printf("Menor numero de comparacoes para o vetor pior caso: %d\n", b_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);
    
    // MARK: Question C

    printf("\n --- C - Resultados QuickSort Recursivo Hibrido com Mediana de 3 ---\n");

    int *c_best_results = get_best_tam_array(random_vet, TAM_ARRAY, true, true);
    int c_best_tam = c_best_results[0];
    printf("\nMelhor tamanho de sub-vetor aleatorio: %d\n", c_best_tam);
    inicio = clock();
    int c_lowest_comp = get_average_result(random_vet, TAM_ARRAY, c_best_tam, 100, true, true);
    fim = clock();
    int c_lowest_com = c_lowest_comp;
    printf("Menor numero de comparacoes para o vetor aleatorio: %d\n", c_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *c_best_results_repeated = get_best_tam_array(repeated_vet, TAM_ARRAY, true, true);
    c_best_tam = c_best_results_repeated[0];
    printf("Melhor tamanho de sub-vetor repetido: %d\n", c_best_tam);
    inicio = clock();
    int c_lowest_comp_repeated = get_average_result(repeated_vet, TAM_ARRAY, c_best_tam, 100, true, true);
    fim = clock();
    c_lowest_com = c_lowest_comp_repeated;
    printf("Menor numero de comparacoes para o vetor repetido: %d\n", c_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *c_best_results_reversed = get_best_tam_array(reversed_vet, TAM_ARRAY, true, true);
    c_best_tam = c_best_results_reversed[0];
    printf("Melhor tamanho de sub-vetor reverso: %d\n", c_best_tam);
    inicio = clock();
    int c_lowest_comp_reversed = get_average_result(reversed_vet, TAM_ARRAY, c_best_tam, 100, true, true);
    fim = clock();
    c_lowest_com = c_lowest_comp_reversed;
    printf("Menor numero de comparacoes para o vetor reverso: %d\n", c_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *c_best_results_sorted = get_best_tam_array(sorted_vet, TAM_ARRAY, true, true);
    c_best_tam = c_best_results_sorted[0];
    printf("Melhor tamanho de sub-vetor ordenado: %d\n", c_best_tam);
    inicio = clock();
    int c_lowest_comp_sorted = get_average_result(sorted_vet, TAM_ARRAY, c_best_tam, 100, true, true);
    fim = clock();
    c_lowest_com = c_lowest_comp_sorted;
    printf("Menor numero de comparacoes para o vetor ordenado: %d\n", c_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *c_best_results_worst_case = get_best_tam_array(worst_case_vet, TAM_ARRAY, true, true);
    c_best_tam = c_best_results_worst_case[0];
    printf("Melhor tamanho de sub-vetor pior caso: %d\n", c_best_tam);
    inicio = clock();
    int c_lowest_comp_worst_case = get_average_result(worst_case_vet, TAM_ARRAY, c_best_tam, 100, true, true);
    fim = clock();
    c_lowest_com = c_lowest_comp_worst_case;
    printf("Menor numero de comparacoes para o vetor pior caso: %d\n", c_lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    printf("\nPressione ENTER para sair...");
    getchar();
    return 0;
}