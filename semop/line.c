#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>

#define NSEM 5

//процесс сборочной линии
int main(void) {
    key_t key = ftok("connection_file.txt", 'a');
    int semaphore = semget(key, NSEM, 0666);
    //printf("1\n");
    struct sembuf operation = {.sem_flg=0, .sem_op=-1};
    for (int counter = 1; semctl(semaphore, NSEM - 1, GETVAL, 0) == 0; counter++) {
        for (int i = 2; i < 4; i++) {
            operation.sem_num = i;
            semop(semaphore, &operation, 1);
        }
        printf("Product №%d is ready\n", counter);
        fflush(stdout);
    }
    return 0;
}