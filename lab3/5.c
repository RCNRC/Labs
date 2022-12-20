#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char * argv[]) {
    /*
    Использование:
    Запустить программу без параметров и ждать завершения родительского процесса.
    */
    int pid;
    if((pid = fork()) == -1) {
        perror("Fork error");
        return -1;
    }
    if(pid){
        if(kill(pid, SIGUSR1) == -1)
            perror("Kill error");
        int code;
        if(wait(&code) == -1)
            perror("Wait error");
        printf("Parent: exit status = %d\n", code);
    }
    else{
        printf("Child: начинает итерации\n");
        for(int i = 0;i < 100;i++){
            for(int j = 0; j < 10000 ; j++);
            printf("%d ",i);
        }
        printf("\n");
    }
    return 0;
}

/*
Задание 5:
В родительском сразу посылаем дочернему сигнал на завершение из-за чего дочерний даже не успевает что-либо вывести.
*/