#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include <string.h>

#define MESSAGE_TYPE 1
#define MESSAGE_DIGIT_INDEX 0
#define MESSAGE_DIGIT_VALUE 1

struct message
{
    long type;
    char data[2];
};

int count_matches(const char *, const int, const int);

void signal_handler(int signum)
{
    exit(EXIT_SUCCESS);
}

int main()
{
    struct sigaction sig_action = { 0 };
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags = 0;
    sig_action.sa_restorer = &signal_handler;
    sigaction(SIGUSR1, &sig_action, 0);

    alarm(20);

    const int num_digits = 10;
    int digits[num_digits];
    memset(digits, 0, sizeof(int) * num_digits);

    char string[63];
    printf("Enter a string: ");
    scanf("%s", string);

    int str_length = strlen(string);
    int msgid = msgget(IPC_PRIVATE, IPC_PRIVATE | 0666);

    if (msgid < 0 )
    {
        fprintf(stdout,"error: can't <msgid> is 0");
        return EXIT_FAILURE;
    }

    struct message buffer;

    for (int i = 0; i < num_digits; ++i)
    {
        pid_t child_pid = fork();
        if (child_pid == 0)
        {
            struct sigaction sig_action = { 0 };
            sigemptyset(&sig_action.sa_mask);
            sig_action.sa_flags = 0;
            sig_action.sa_restorer = &signal_handler;
            sigaction(SIGUSR1, &sig_action, 0);

            int matches = count_matches(string, str_length, i);

            buffer.type = MESSAGE_TYPE;
            buffer.data[MESSAGE_DIGIT_INDEX] = i;
            buffer.data[MESSAGE_DIGIT_VALUE] = matches;
            msgsnd(msgid, &buffer, sizeof(buffer.data), 0);

            kill(getpid(), SIGUSR2);
        }
    }

    while (wait(NULL) > 0);

    for (int i = 0; i < num_digits; i++)
    {
        msgrcv(msgid, &buffer, sizeof(buffer.data), 0, 0);
        int index = buffer.data[MESSAGE_DIGIT_INDEX];
        int matches = buffer.data[MESSAGE_DIGIT_VALUE];
        digits[index] = matches;
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