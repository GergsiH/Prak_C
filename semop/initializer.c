#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>

#define NSEM 5

key_t key;
int semaphore;

void handler(int sig) {
    if (sig == SIGINT) {
        printf("Facility is closing\n");
        struct sembuf exit = {.sem_num=(NSEM - 1), .sem_flg=0, .sem_op=1};
        semop(semaphore, &exit, 1);
    }
}

int main(void) {
//    printf("%d\n", getpid());
    key = ftok("connection_file.txt", 'a');
    semaphore = semget(key, NSEM, 0666 | IPC_CREAT);
    sigaction(SIGINT, &(struct sigaction) {.sa_handler=handler, .sa_flags=SA_RESTART}, NULL);
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGINT);
    int mas[6] = {0, 0, 0, 0, 0, 0};
    semctl(semaphore, 0, SETALL, mas);
    //for (int i = 0; i < NSEM; i++) {
      //  semctl(semaphore, i, GETVAL, 0);
    //}
    printf("Facility is ready\n");
    sigsuspend(&mask);
    semctl(semaphore, 0, IPC_RMID, 0);
    return 0;
}