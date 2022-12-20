#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int MAX = 3;

void signal_handler(int signal_id){
    printf("Сигнал SIGINT перехвачен.\n");
    MAX = 6;
}

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
        sleep(1);
        if(kill(pid, SIGINT) == -1)
            perror("Kill error");
        
        int code;
        if(wait(&code) == -1)
            perror("Wait error");
        printf("Parent: exit status = %d\n", code);
    }
    else{
        if(signal(SIGINT, signal_handler)==SIG_ERR)
            perror("Signal handler");
        for(int i = 0;i < MAX;i++){
            sleep(1);
            printf("%d ", i);
        }
        printf("\nChild: завершил выполнение.\n");
    }
    return 0;
}

/*
Задание 8:
В родительском спустя 1 сек посылаем дочернему сингал SIGINT.
Тот в свою очередь переопределяет его на хэндлер, где задаётся другое число для итерации.
В итоге дочерний выведет 6 итераций, а не 3, как было изначально.
*/