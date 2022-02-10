#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#define MY_KEY 1998

struct msgbuffer
{
    int type;
    int data;
};

int main()
{
    int msgid = msgget(64, 0666 | IPC_EXCL);

    if (msgid < 0 )
    {
        fprintf(stdout,"error: {msgid} is 0");
        return EXIT_FAILURE;
    }

    struct msgbuffer buffer;

    msgrcv(msgid, &buffer, 2, 0, 0);

    return 0;
}