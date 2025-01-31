#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>

#define NSEM 5

//процесс создания детали B
int main(void) {
    key_t key = ftok("connection_file.txt", 'a');
    int semaphore = semget(key, NSEM, 0666);
    struct sembuf operation = {.sem_num=1, .sem_flg=0, .sem_op=1};
    for (int counter = 1; semctl(semaphore, NSEM - 1, GETVAL, 0) == 0; counter++) {
        printf("I'm starting production of detail 'B' №%d\n", counter);
        sleep(3);
        printf("Detail 'B' №%d is ready\n", counter);
        semop(semaphore, &operation, 1);
    }
    return 0;
}