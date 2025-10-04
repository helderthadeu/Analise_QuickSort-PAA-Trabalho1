#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * @brief Gera um array de 'n' inteiros aleatórios entre 'lower_bound' e 'upper_bound'.
 * * @param n O número de inteiros a gerar.
 * @param lower_bound O limite inferior (inclusivo).
 * @param upper_bound O limite superior (inclusivo).
 * @return Um ponteiro para o array alocado dinamicamente, ou NULL em caso de falha.
 */
int* generate_tests(int n, int lower_bound, int upper_bound) {
    int* mass = (int*)malloc(n * sizeof(int));
    if (mass == NULL) {
        perror("Falha ao alocar memória para o array de teste");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        mass[i] = (rand() % (upper_bound - lower_bound + 1)) + lower_bound;
    }
    return mass;
}

/**
 * @brief Função de comparação para qsort para ordenação ascendente.
 */
int compare_asc(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

/**
 * @brief Função de comparação para qsort para ordenação descendente.
 */
int compare_desc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

/**
 * @brief Escreve o conteúdo de um array de inteiros em um arquivo.
 * * @param filepath O caminho do arquivo.
 * @param mass O array a ser escrito.
 * @param size O tamanho do array.
 * @return 0 em caso de sucesso, -1 em caso de falha.
 */
int write_mass_to_file(const char* filepath, const int* mass, int size) {
    FILE* file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo para escrita");
        return -1;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", mass[i]);
    }

    fclose(file);
    return 0;
}

// /**
//  * @brief Imprime o conteúdo de um array de inteiros no console.
//  * * @param mass O array a ser impresso.
//  * @param size O tamanho do array.
//  */
// void print_mass(const char* title, const int* mass, int size) {
//     printf("%s [", title);
//     for (int i = 0; i < size; i++) {
//         printf("%d", mass[i]);
//         if (i < size - 1) {
//             printf(", ");
//         }
//     }
//     printf("]\n");
// }

/**
 * @brief Constrói um array que gera pior caso até mesmo para QuickSort com pivô aleatório.
 * Implementação baseada no método "anti-quicksort" descrito por Sedgewick e Bentley.
 */
void build_worst_case_array(int *arr, int n) {
    int *pos = (int*)malloc(n * sizeof(int));
    if (!pos) {
        perror("Falha ao alocar memória em build_worst_case_array");
        return;
    }

    int random = rand() % n;

    for (int i = 0; i < n; i++) {
        arr[i] = random;
    }
    
    free(pos);
}


int generate_tests_mass(const int N) {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    const int LOWER_BOUND = 0;
    const int UPPER_BOUND = 100;

    int* test_mass = generate_tests(N, LOWER_BOUND, UPPER_BOUND);
    if (test_mass == NULL) {
        return 1; // Encerra se a alocação de memória falhou
    }
    
    // Aloca memória para os arrays auxiliares
    int* sorted_mass = (int*)malloc(N * sizeof(int));
    int* reversed_mass = (int*)malloc(N * sizeof(int));
    if (sorted_mass == NULL || reversed_mass == NULL) {
        perror("Falha ao alocar memória para os arrays auxiliares");
        free(test_mass);
        free(sorted_mass);
        free(reversed_mass);
        return 1;
    }

    // Copia o array original para os auxiliares
    memcpy(sorted_mass, test_mass, N * sizeof(int));
    memcpy(reversed_mass, test_mass, N * sizeof(int));
    
    // --- Salvar tamanho ---
    FILE* size_file = fopen("tests/size_test_mass.txt", "w");
    if (size_file != NULL) {
        fprintf(size_file, "%d", N);
        fclose(size_file);
    } else {
        perror("Erro ao criar tests/size_test_mass.txt");
    }

    // --- Salvar massa aleatória ---
    write_mass_to_file("tests/random_test_mass.txt", test_mass, N);

    // --- Salvar massa ordenada ---
    qsort(sorted_mass, N, sizeof(int), compare_asc);
    // print_mass("Massa Ordenada:", sorted_mass, N);
    write_mass_to_file("tests/sorted_test_mass.txt", sorted_mass, N);
    
    // --- Salvar massa invertida ---
    qsort(reversed_mass, N, sizeof(int), compare_desc);
    write_mass_to_file("tests/reversed_test_mass.txt", reversed_mass, N);

    // --- Salvar massa com repetições ---
    const int REPEATED_SIZE = 20;
    int intermediate_mass[REPEATED_SIZE];
    for (int i = 0; i < REPEATED_SIZE; i++) {
        intermediate_mass[i] = test_mass[rand() % N];
    }
    
    int repeated_mass[N];
    for (int i = 0; i < N; i++) {
        repeated_mass[i] = intermediate_mass[rand() % REPEATED_SIZE];
    }
    // print_mass("Massa Repetida:", repeated_mass, REPEATED_SIZE);
    write_mass_to_file("tests/repeated_test_mass.txt", repeated_mass, N);

        // --- Salvar massa de pior caso "anti-quicksort" ---
    int* anti_mass = (int*)malloc(N * sizeof(int));
    if (anti_mass == NULL) {
        perror("Falha ao alocar memória para worst_case");
    } else {
        build_worst_case_array(anti_mass, N);
        write_mass_to_file("tests/worst_case_test_mass.txt", anti_mass, N);
    }


    printf("\nMassas de teste geradas e salvas na pasta 'tests'.\n");
    // print_mass("Massa Original:", test_mass, N);

    // Libera a memória alocada dinamicamente
    free(test_mass);
    free(sorted_mass);
    free(reversed_mass);
    free(anti_mass);

    return 0;
}
