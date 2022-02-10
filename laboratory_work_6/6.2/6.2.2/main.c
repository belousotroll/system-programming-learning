#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "../message.h"

int main()
{
    int msgid = msgget(200, IPC_EXCL | 0666);
    if (msgid < 0)
    {
        fprintf(stderr,"error: msgid is 0");
        return EXIT_FAILURE;
    }

    struct message buffer;

    msgrcv(msgid, &buffer, sizeof(buffer), 0, 0);

    const int n = MATRIX_DIMENSION * MATRIX_DIMENSION;
    for (int i = 0; i < n; ++i)
    {
        printf("%c ", buffer.data[i]);
        if ((i + 1) % 4 == 0)
        {
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}