#ifndef LABORATORY_WORK_5_SHARED_MEMORY_H
#define LABORATORY_WORK_5_SHARED_MEMORY_H

#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <assert.h>

#define IPC_RESULT_ERROR (-1)

void * get_shared_memory(int size, int * shmid)
{
    assert(shmid);

    *shmid = shmget(IPC_PRIVATE, size, 0644 | IPC_CREAT);

    if (*shmid == IPC_RESULT_ERROR)
    {
        return NULL;
    }

    char * result = shmat(*shmid, NULL, 0);
    if (result == (char *)(IPC_RESULT_ERROR))
    {
        return NULL;
    }

    return result;
}

bool detach_shared_memory(char * memory)
{
    return (shmdt(memory) != IPC_RESULT_ERROR);
}

bool free_shared_memory(char * filename)
{
    int shmid = get_shared_memory(filename, 0);

    if (shmid == IPC_RESULT_ERROR)
    {
        return NULL;
    }

    return (shmctl(shmid, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}

#endif //LABORATORY_WORK_5_SHARED_MEMORY_H
