#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[], char * argp[]){
    int fds[2];
    pipe(fds);
    if(fork() == 0){
        printf("[C-WHO] Дочерний процесс запущен! PID = %d\n", getpid());
        close(1);
        close(fds[0]);
        dup2(fds[1], 1);
        execvp("/usr/bin/who", argv);
        perror("[C-WHO] execvp error");
    }

    if(fork() == 0){
        printf("[C-WC] Дочерний процесс запущен! PID = %d\n", getpid());
        char buf;
        close(0);
        close(fds[1]);
        dup2(fds[0], 0);
        char * args[] = {"/usr/bin/wc", "-l", NULL};
        execvp("/usr/bin/wc", args);
        perror("[C-WC] execvp error");
    }
    close(fds[0]);
    close(fds[1]);
    return 1;
}