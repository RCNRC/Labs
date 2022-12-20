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
        sleep(1);  // показываю, что дочерний на паузе ждёт 1 сек.
        if(kill(pid, SIGINT) == -1)
            perror("Kill error");
        int code;
        if(wait(&code) == -1)
            perror("Wait error");
        printf("Parent: exit status = %d\n", code);
    }
    else{
        pause();
        printf("Child: эта строка напечатана не будет, т.к. получен сигнал SIGINT на завершение.\n");
    }
    return 0;
}

/*
Задание 7:
Единственное отличие - при паузе дочерний просто будет ждать сигнала, после которого завершится. 
*/