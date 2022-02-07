#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../shared_memory.h"

struct node_t // Узел
{
    char * begin; // Указатель на начало строки
    char * end; // Указатель на конец строки
    struct node_t * next; // Указатель на следующий элемент
};

bool is_polindrome(char *, char *);
void print_ipcs_memory_table(const int);
struct node_t * create_list_of_words(const char *, const size_t, size_t *);
void free_list(struct node_t *);

int main(int argc, char ** argv)
{
    size_t buffer_v;

    printf("Enter couple of words: ");
    char * string = NULL;
    size_t string_length = getline(&string, &buffer_v, stdin) - 2;

    const char space_token = ' ';

    size_t num_words = 0; // содержим в себе количесто слов в строке

    // Делит строку типа "привет как дела" на отдельные слова.
    struct node_t * current = create_list_of_words(string, string_length, &num_words);
    // Указатель на самое первое слово.
    struct node_t * head = current;

    // [ _ _ _ ]

    int shmid;
    char * shared_memory = get_shared_memory(sizeof(char) * num_words, &shmid);
    print_ipcs_memory_table(shmid);

    for (int i = 0; i < num_words; ++i)
    {
        if (fork() == 0)
        {
            bool is_correct = is_polindrome(current->begin, current->end);
            size_t length = (current->end - current->begin) + 1;

            // "привет как дела"
            // [ 0 3 0]

            shared_memory[i] = (is_correct)
                               ? length
                               : 0;

            exit(EXIT_SUCCESS);
        }

        current = current->next;
    }

    // Ждем завершение всех дочерних процессов.
    while (wait(NULL) != -1);

    int max = 0;
    for (int i = 0; i < num_words; ++i)
    {
        if (shared_memory[i] > max)
        {
            max = shared_memory[i];
        }
    }

    printf("result: %d\n", max);

    detach_shared_memory(shared_memory);
    free_list(head);

    return 0;
}

struct node_t * create_list_of_words(const char * string, const size_t length, size_t * num_words)
{
    *num_words = 0;

    const char space_token = ' ';

    struct node_t * node = malloc(sizeof(struct node_t));
    struct node_t * head = node;

    char * left_border = NULL;
    char * right_border = NULL;

    for (int i = 0; i < length; ++i)
    {
        while (string[i] == space_token && i != length)
        {
            ++i;
        }

        left_border = &string[i];

        while (string[i + 1] != space_token && i != length)
        {
            ++i;
        }

        right_border = &string[i];

        node->begin = left_border;
        node->end = right_border;

        ++*num_words;

        if (i == length)
        {
            break;
        }

        node->next = malloc(sizeof(struct node_t));
        node = node->next;
    }

    return head;
}

bool is_polindrome(char * left_border, char * right_border)
{
    char * begin = left_border;
    char * end = right_border;

    while(begin <= end)
    {
        if(*begin != *end)
        {
            return false;
        }

        ++begin;
        --end;
    }

    return true;
}

void print_ipcs_memory_table(const int shmid)
{
    char command[32];
    snprintf(command, sizeof(command), "ipcs -m -i %d", shmid);
    system(command);
}

void free_list(struct node_t * head)
{
    struct node_t * current = head;
    struct node_t * next = head->next;

    while (next != NULL)
    {
        free(current);

        current = next;
        next = next->next;
    }
}