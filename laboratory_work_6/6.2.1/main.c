#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#define PROJECT_ID 'M'
#define MY_KEY 1998

struct msgbuffer
{
    long  type;
    int  data;
};

int main()
{
    printf("Enter a number: ");
    int number;
    fscanf(stdin, "%d", &number);

    int msgid = msgget(200, 0666 | IPC_CREAT);

    if (msgid < 0 )
    {
        fprintf(stdout,"error: {msgid} is 0");
        return EXIT_FAILURE;
    }

    struct msgbuffer buffer;

    buffer.type = 1;
    buffer.data = number;

    msgsnd(msgid, &buffer, sizeof(buffer), 0);

    return 0;
}