#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../shared_memory.h"

int ** create_matrix(const int);
void remove_matrix(int **, const int);

int calculate_first_task(int **, const size_t);
int calculate_second_task(int **, const size_t);

void print_ipcs_memory_table(const int);

void fill_matrix(FILE *, int **, const size_t);

int main(int argc, char ** argv)
{
    int dimension;

    const char * filename = argv[1];

    FILE * in = fopen(filename, "r");

    fscanf(in, "%d", &dimension);

    int shmid;
    char * shared_memory = get_shared_memory(sizeof(char) * 2, &shmid);
    print_ipcs_memory_table(shmid);

    int ** matrix = create_matrix(dimension);
    fill_matrix(in, matrix, dimension);

    if (fork() == 0)
    {
        shared_memory[0] = calculate_first_task(matrix, dimension);
        exit(EXIT_SUCCESS);
    }

    if (fork() == 0)
    {
        shared_memory[1] = calculate_second_task(matrix, dimension);
        exit(EXIT_SUCCESS);
    }

    // Ждем завершение всех дочерних процессов.
    while (wait(NULL) != -1);

    printf("1st task result: %d\n", shared_memory[0]);
    printf("2nd task result: %d\n", shared_memory[1]);

    remove_matrix(matrix, dimension);
    detach_shared_memory(shared_memory);
    fclose(in);

    return 0;
}

int ** create_matrix(const int dimension)
{
    int ** matrix = malloc(sizeof(int *) * dimension);

    for (int i = 0; i < dimension; ++i)
    {
        matrix[i] = malloc(sizeof(int) * dimension);
    }

    return matrix;
}

void remove_matrix(int ** matrix, const int dimension)
{
    for (int i = 0; i < dimension; ++i)
    {
        free(matrix[i]);
    }

    free(matrix);
}

void fill_matrix(FILE * file, int ** matrix, const size_t dimension)
{
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
}

int calculate_first_task(int ** matrix, size_t dimension)
{
    int result = 0;

    //     0 1 2 3
    // 0 [ 0 1 2 3 ] - i
    // 1 [ 0 1 2 3 ]
    // 2 [ 0 1 2 3 ]
    // 3 [ 0 1 2 3 ]
    //     j

    // Сумма больше, сумма всех положительных чисел, стоящих выше главной диагонали.
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = i + 1; j < dimension; ++j)
        {
            if (matrix[i][j] > 0)
            {
                result += matrix[i][j];
            }
        }
    }

    // Сумма больше, сумма всех положительных чисел, стоящих ниже второй главной диагонали.
    for (int i = 1; i < dimension; ++i)
    {
        for (int j = dimension - 1; j > (dimension - 1) - i; --j)
        {
            if (matrix[i][j] > 0)
            {
                result += matrix[i][j];
            }
        }
    }

    return result;
}

int calculate_second_task(int ** matrix, size_t dimension)
{
    int result = 0;

    //     0 1 2 3
    // 0 [ 0 1 2 3 ] - i
    // 1 [ 0 1 2 3 ]
    // 2 [ 0 1 2 3 ]
    // 3 [ 0 1 2 3 ]
    //     j

    // Cумма модулей всех отрицательных чисел, стоящих выше второй главной диагонали.
    for (int i = 0 ; i < dimension; ++i)
    {
        for (int j = 0; j > (dimension - 2) - i; ++j)
        {
            if (matrix[i][j] < 0)
            {
                result += abs(matrix[i][j]);
            }
        }
    }

    // Cумма модулей всех отрицательных чисел, стоящих ниже главной диагонали.
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 1 + i; j < dimension; ++j)
        {
            if (matrix[i][j] < 0)
            {
                result += abs(matrix[i][j]);
            }
        }
    }

    return result;
}

void print_ipcs_memory_table(const shmid)
{
    char command[32];
    snprintf(command, sizeof(command), "ipcs -m -i %d", shmid);
    system(command);
}
