#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "../message.h"

void fill_matrix(FILE *, char *);

int main()
{
    FILE * fd = fopen("resource/matrix.txt", "r");
    if (fd == NULL)
    {
        fprintf(stderr,"error: cannot open file resource/matrix.txt");
        return EXIT_FAILURE;
    }

    int msgid = msgget(200, IPC_EXCL | 0666);
    if (msgid < 0)
    {
        fprintf(stderr,"error: msgid is 0");
        return EXIT_FAILURE;
    }

    struct message buffer;

    buffer.type = 0;
    fill_matrix(fd, buffer.data);

    const int n = MATRIX_DIMENSION * MATRIX_DIMENSION;
    for (int i = 0; i < n; ++i)
    {
        printf("%c ", buffer.data[i]);
        if ((i + 1) % 4 == 0)
        {
            printf("\n");
        }
    }


    msgsnd(msgid, &buffer, sizeof(buffer), 0);

    fclose(fd);

    return 0;
}

void fill_matrix(FILE * fd, char * array)
{
    const int n = MATRIX_DIMENSION * MATRIX_DIMENSION;
    for (int i = 0; i < n; ++i) {
        fscanf (fd, "%c", &array[i]);
    }
}