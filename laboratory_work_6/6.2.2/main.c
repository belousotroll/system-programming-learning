#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuffer
{
    long type;
    int data;
};

int main()
{
    int msgid = msgget(200, 0666 | IPC_EXCL);

    if (msgid < 0 )
    {
        fprintf(stdout,"error: {msgid} is 0");
        return EXIT_FAILURE;
    }

    struct msgbuffer buffer;

    msgrcv(msgid, &buffer, sizeof(buffer), 0, 0);

    printf("Number1 is %d\n", buffer.data);

    struct msgbuffer sndbuffer;

    sndbuffer.type = 1;
    sndbuffer.data = buffer.data * 2;

    msgsnd(msgid, &sndbuffer, sizeof(sndbuffer), 0);

    printf("Number2 is %d\n", sndbuffer.data);

    return 0;
}