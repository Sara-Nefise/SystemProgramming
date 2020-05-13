#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
    int filedes2[2];
    int filedes1[2];
    int filedes3[2];
    int filedes4[2];

    pipe2(filedes2,O_CLOEXEC);
    pipe2(filedes1,O_CLOEXEC);
    pipe2(filedes3,O_CLOEXEC);
    pipe2(filedes4,O_CLOEXEC);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(filedes1[1], 1);

        execlp("ls", "ls", "-1", (char *)NULL);
        exit(0);
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(filedes1[0], 0);
        dup2( filedes2[1],1);
        execlp("cut", "cut", "-d.", "-f1", (char *)NULL);
    }
    pid_t pid2 = fork();
    if (pid2> 0) {
        dup2(filedes2[0], 0);
        dup2(filedes3[1],1);
        execlp("sort", "sort", (char *)NULL);

    }
    pid_t pid3 = fork();
    if (pid3> 0) {
        dup2(filedes3[0],0);
        dup2(filedes4[1],1);
        execlp("uniq", "uniq", (char *)NULL);
    }
    pid_t pid4 = fork();
    if (pid4> 0) {
        dup2(filedes4[0],0);
        execlp("tee", "tee","dirents", (char *)NULL);

    }


    return 0;
}

