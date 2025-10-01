#include <stdio.h>
#include <stdlib.h> // srand, rand
#include <time.h>   // time
#include "utils.c"

#define INT_MAX 2147483647

int TAM_MAX_ARRAY = 50;
int num_comparacoes = 0;

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

// função que realiza a troca entre dois elementos
void troca(int vet[], int i, int j)
{
    int aux = vet[i];
    vet[i] = vet[j];
    vet[j] = aux;
}

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

void quick_sort(int vet[], int inicio, int fim)
{
    num_comparacoes++;
    if (fim - inicio <= TAM_MAX_ARRAY)
    {
        num_comparacoes--;
        // printf("Usando Insertion Sort\n");
        insertionSort(vet, inicio, fim + 1);
    }
    else if (inicio < fim)
    {
        // função particionar retorna o índice do pivô
        int pivo_indice = particiona_random(vet, inicio, fim);

        // chamadas recursivas quick_sort
        quick_sort(vet, inicio, pivo_indice - 1);
        quick_sort(vet, pivo_indice + 1, fim);
    }
    num_comparacoes++;
}

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

int *get_best_tam_array(int vet[], int tam_vet)
{
    int best_tam = 0;
    int lowest_com = INT_MAX;
    int *best_results = (int *)malloc(2 * sizeof(int));

    for (int j = 1; j <= 200; j++)
    {
        num_comparacoes = 0;
        TAM_MAX_ARRAY = j;

        // chamada do quicksort
        quick_sort(vet, 0, tam_vet - 1);

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

int get_average_result(int vet[], int tam_vet, int TAM_MAX_ARRAY, int num_executions)
{
    int best_tam = 0;
    int lowest_com = INT_MAX;

    for (int i = 0; i < num_executions; i++)
    {
        num_comparacoes = 0;

        // chamada do quicksort
        quick_sort(vet, 0, tam_vet - 1);

        // printf("Tamanho do sub-vetor: %d \t-\t Numero de comparacoes: %d\n", TAM_MAX_ARRAY, num_comparacoes);
        if (num_comparacoes < lowest_com)
        {
            lowest_com = num_comparacoes;
            best_tam = TAM_MAX_ARRAY;
        }
    }

    return num_comparacoes / num_executions;
}

int main()
{
    clock_t inicio, fim;
    double tempo_gasto;

    generate_tests_mass(10000);

    int TAM_ARRAY = 0;
    FILE *file;

    file = fopen("tests\\size_test_mass.txt", "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(file, "%d", &TAM_ARRAY);

    fclose(file);

    int *random_vet = open_file_and_read_array("tests\\random_test_mass.txt", TAM_ARRAY);
    int *repeated_vet = open_file_and_read_array("tests\\repeated_test_mass.txt", TAM_ARRAY);
    int *reversed_vet = open_file_and_read_array("tests\\reversed_test_mass.txt", TAM_ARRAY);
    int *sorted_vet = open_file_and_read_array("tests\\sorted_test_mass.txt", TAM_ARRAY);
    int *worst_case_vet = open_file_and_read_array("tests\\worst_case_test_mass.txt", TAM_ARRAY);

    // printf("random_vet: %d\n", random_vet[0]);
    int *best_results = get_best_tam_array(random_vet, TAM_ARRAY);
    int best_tam = best_results[0];
    printf("\nMelhor tamanho de sub-vetor aleatorio: %d\n", best_tam);
    inicio = clock();
    int lowest_comp = get_average_result(random_vet, TAM_ARRAY, best_tam, 100);
    fim = clock();
    int lowest_com = lowest_comp;
    printf("Menor numero de comparacoes para o vetor aleatorio: %d\n", lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *best_results_repeated = get_best_tam_array(repeated_vet, TAM_ARRAY);
    best_tam = best_results_repeated[0];
    printf("Melhor tamanho de sub-vetor repetido: %d\n", best_tam);
    inicio = clock();
    int lowest_comp_repeated = get_average_result(repeated_vet, TAM_ARRAY, best_tam, 100);
    fim = clock();
    lowest_com = lowest_comp_repeated;
    printf("Menor numero de comparacoes para o vetor repetido: %d\n", lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *best_results_reversed = get_best_tam_array(reversed_vet, TAM_ARRAY);
    best_tam = best_results_reversed[0];
    printf("Melhor tamanho de sub-vetor reverso: %d\n", best_tam);
    inicio = clock();
    int lowest_comp_reversed = get_average_result(reversed_vet, TAM_ARRAY, best_tam, 100);
    fim = clock();
    lowest_com = lowest_comp_reversed;
    printf("Menor numero de comparacoes para o vetor reverso: %d\n", lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *best_results_sorted = get_best_tam_array(sorted_vet, TAM_ARRAY);
    best_tam = best_results_sorted[0];
    printf("Melhor tamanho de sub-vetor ordenado: %d\n", best_tam);
    inicio = clock();
    int lowest_comp_sorted = get_average_result(sorted_vet, TAM_ARRAY, best_tam, 100);
    fim = clock();
    lowest_com = lowest_comp_sorted;
    printf("Menor numero de comparacoes para o vetor ordenado: %d\n", lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    int *best_results_worst_case = get_best_tam_array(worst_case_vet, TAM_ARRAY);
    best_tam = best_results_worst_case[0];
    printf("Melhor tamanho de sub-vetor pior caso: %d\n", best_tam);
    inicio = clock();
    int lowest_comp_worst_case = get_average_result(worst_case_vet, TAM_ARRAY, best_tam, 100);
    fim = clock();
    lowest_com = lowest_comp_worst_case;
    printf("Menor numero de comparacoes para o vetor pior caso: %d\n", lowest_com);
    printf("Tempo(s): %f \n\n",(double)(fim - inicio)/CLOCKS_PER_SEC);

    printf("\nPressione ENTER para sair...");
    getchar(); 
    return 0;
}
