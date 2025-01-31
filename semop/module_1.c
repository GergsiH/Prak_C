#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>

#define NSEM 5

//процесс создания module 1
int main(void) {
    key_t key = ftok("connection_file.txt", 'a');
    int semaphore = semget(key, NSEM, 0666);
    struct sembuf operation = {.sem_flg=0};
    for (int counter = 1; semctl(semaphore, NSEM - 1, GETVAL, 0) == 0; counter++) {
        operation.sem_op = -1;
        for (int j = 0; j < 2; j++) {
            operation.sem_num = j;
            semop(semaphore, &operation, 1);
        }
        printf("I'm starting assembling of module_1 №%d\n", counter);
        sleep(1);
        printf("Module_1 №%d is ready\n", counter);
        operation.sem_num = 2;
        operation.sem_op = 1;
        semop(semaphore, &operation, 1);
    }
    return 0;
}