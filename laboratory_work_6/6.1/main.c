#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MESSAGE_TYPE 1
#define MESSAGE_DIGIT_INDEX 0
#define MESSAGE_DIGIT_VALUE 1

struct message
{
    int type;
    int data[2];
};

int count_matches(const char *, const int, const int);

int main()
{
    const int num_proc = 10;
    const int num_digits = 10;

    int digits[num_digits];
    memset(digits, 0, num_digits);

    char string[63];
    printf("Enter a string: ");
    scanf("%s", string);

    int str_length = strlen(string);
    int msgid = msgget(IPC_PRIVATE, IPC_PRIVATE | 0666);

    printf("status: start of program; data :: msgid: %d\n", msgid);

    if (msgid < 0 )
    {
        fprintf(stdout,"error: can't <msgid> is 0");
        return EXIT_FAILURE;
    }

    int num_iterations = ((str_length / num_proc) == 0)
                         ? str_length
                         : num_proc;

    struct message buffer;

    for (int i = 0; i < num_iterations; ++i)
    {
        if (fork() == 0)
        {
            int matches = count_matches(string, str_length, i);

            buffer.type = MESSAGE_TYPE;
            buffer.data[MESSAGE_DIGIT_INDEX] = i;
            buffer.data[MESSAGE_DIGIT_VALUE] = matches;
            msgsnd(msgid, &buffer, sizeof(buffer.data), 0);

            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < num_iterations; i++)
    {
        msgrcv(msgid, &buffer, sizeof(buffer.data), 0, 0);
        int index = buffer.data[MESSAGE_DIGIT_INDEX];
        int value = buffer.data[MESSAGE_DIGIT_VALUE];
        digits[index] += value;
    }

    for (int i = 0; i < num_digits; ++i)
    {
        printf("%d ", digits[i]);
    }
    printf("\n");

    return 0;
}

int count_matches(const char * ptr,
                  const int size,
                  const int num)
{
    int count = 0;
    for (int i = 0; i <= size; ++i)
    {
        const int digit = (int)ptr[i] - 48;
        if (digit == num)
        {
            count++;
        }
    }
    return count;
}