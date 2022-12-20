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
        int code;
        if(wait(&code) == -1)  // ожидаем любой дочерний
            perror("Wait error");
        else
            printf("Parent: exit status = %d\n", code);  // вернёт 0
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
Задание 4:
Родитель будет ждать, пока не пройдут все итерации в дочернем. Затем он вернёт итог работы дочернего.
*/