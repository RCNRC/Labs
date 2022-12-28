#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char * argv []){
    /*
        fds[0] - чтение
        fds[1] - запись
    */
    int fds[2], fds_i[2];
    int pid, l;
    char buf[80];
    if(pipe(fds) == -1){
        perror("pipe error");
    }
    if(pipe(fds_i) == -1){
        perror("pipe error");
    }
    if((pid = fork()) == -1){
        perror("fork error");
    }

    if(pid){
        printf("[P] Дочерний процесс создан! PID = %d\n", pid);
        close(fds[0]);
        close(fds_i[1]);
        close(0);
        dup2(fds[1], 0);
        while ((l=read(fds_i[0], buf, 80))>0)
            write(1, buf, l);
            
        printf("[P] Завершение родительского процесса!\n");
        close(fds[0]);
        close(fds_i[1]);
        wait(NULL);
    }

    else{
        printf("[C] Child process started!");
        close(fds[1]);
        close(fds_i[0]);
        char buf[80];
        close(1);
        dup2(fds_i[1], 1);
        execve("4_exec", 0, NULL);
        perror("execve error");
        
    }
}
/*
Задание 4.
= задание 2. Но через exec. Передаётся дескрипторы их можно дублировать на место дескрипторов по умолчанию.
*/