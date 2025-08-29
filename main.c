#include <stdio.h>
#include <stdlib.h> // srand, rand
#include <time.h>   // time

#define TAM_ARRAY 10000
// #define TAM_MAX_ARRAY 50
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
    int pivo_indice = (rand() % (fim - inicio + 1)) + inicio;

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

int main()
{
    // inicializa random seed
    srand(time(NULL));

    // vetor que será ordenado
    int vet[TAM_ARRAY];

    printf("Vetor original: \n");
    for (int i = 0; i < TAM_ARRAY; i++)
    {
        num_comparacoes++;
        vet[i] = rand() % 100;
        // printf("%d ", vet[i]);
    }

    int best_tam = 0;
    int lowest_com = INT_MAX;
    for (int j = 1; j <= 200; j++)
    {
        num_comparacoes = 0;
        TAM_MAX_ARRAY = j;

        // printf("\nVetor ordenado: \n");

        // int vet[] = {25, 40, 55, 20, 44, 35, 38, 99, 10, 65, 50};
        int tam_vet = sizeof(vet) / sizeof(int);
        int i;

        // chamada do quicksort
        quick_sort(vet, 0, tam_vet - 1);

        // for (i = 0; i < tam_vet; i++)
        // {
        //     printf("%d ", vet[i]);
        // }
        printf("Tamanho do sub-vetor: %d \t-\t Numero de comparacoes: %d\n", TAM_MAX_ARRAY, num_comparacoes);
        if (num_comparacoes < lowest_com)
        {
            lowest_com = num_comparacoes;
            best_tam = TAM_MAX_ARRAY;
        }
    }
    printf("\nMelhor tamanho de sub-vetor: %d \t-\t Numero de comparacoes: %d\n", best_tam, lowest_com);
    return 0;
}