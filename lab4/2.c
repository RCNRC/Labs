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
        int num = 10;
        while ((l=read(fds_i[0], buf, 80))>0)
            write(1, buf, l);

        printf("[P] Завершение родительского процесса!\n");
        close(fds[0]);
        close(fds_i[1]);
        wait(NULL);
    }

    else{
        close(fds[1]);
        close(fds_i[0]);
        int another_num;
        while((l=read(0, buf, 80))>0)
            write(fds_i[1], buf, l);
        
        printf("[C] Завершение дочернего процесса!\n");
        close(fds[1]);
        close(fds_i[0]);
        
    }
}
/*
Задание 2.
Родитель бесконечно читает от сына по пайпу и сразу печатает в терминал.
Можно завершить на сигнал ctrl+D
*/