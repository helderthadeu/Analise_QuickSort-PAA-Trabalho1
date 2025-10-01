# Análise do Algortimo QuickSort
Grupo: Helder Thadeu, Gabriel Vieira e Enzo Rodrigues

# Descrição

Este projeto implementa uma análise do algoritmo QuickSort, avaliando seu desempenho em diferentes tipos de arrays de entrada. O objetivo é medir:

O número de comparações realizadas pelo QuickSort.

O tempo de execução.

O comportamento do algoritmo em arrays aleatórios, ordenados, inversamente ordenados e com repetições.

O código gera automaticamente arrays de teste e salva os resultados em arquivos para posterior análise.

# Estrutura do Projeto
main.c               # Programa principal
utils.c              # Funções auxiliares (geração de arrays, escrita de arquivos, etc.)
tests/               # Pasta onde os arquivos de teste e resultados são salvos
    random_test_mass.txt
    sorted_test_mass.txt
    reversed_test_mass.txt
    repeated_test_mass.txt
    size_test_mass.txt
README.md

# Funcionalidades

Geração de arrays de teste

Aleatórios: valores entre LOWER_BOUND e UPPER_BOUND.

Ordenados: crescente e decrescente.

Com repetições: sub-arrays gerados aleatoriamente.

Medida de desempenho

Número de comparações do QuickSort.

Tempo de execução em segundos usando clock() da biblioteca <time.h>.

Saída organizada em arquivos

Cada tipo de array é salvo em um arquivo separado na pasta tests/.

# Como Compilar e Executar
Usando GCC no terminal:
gcc main.c utils.c -o main.exe
./main.exe

Usando VS Code

Configure o launch.json com "externalConsole": true para ver a saída no console.

Execute com F5 (debug) ou Ctrl+F5 (rodar sem debug).

Dica: o programa espera uma entrada final (getchar() ou system("pause")) para que o terminal não feche imediatamente, permitindo ler os resultados.

# Saída do Programa

Exemplo de saída:

Melhor tamanho de sub-vetor aleatorio: 50
Menor numero de comparacoes para o vetor aleatorio: 2345
Tempo(s): 0.123456

Os arquivos na pasta tests/ contêm os arrays utilizados e podem ser utilizados para análises adicionais.

Observações

O código está preparado para ajustar facilmente o tamanho do array de teste (TAM_ARRAY) e o número de execuções (N_EXECUTIONS) para avaliação estatística.

Para evitar fechar o terminal imediatamente, recomenda-se manter um getchar() ao final do main().
