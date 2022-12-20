#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int MAX = 3;

void alarm_handler(int signal_id){
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
        int code;
        if(wait(&code) == -1)
            perror("Wait error");
        printf("Parent: exit status = %d\n", code);
    }
    else{
        if(signal(SIGALRM, alarm_handler)==SIG_ERR) {perror("Alarm handler");}  // закомментировать, чтобы аларм сработал по дефолту
        alarm(2);  // через 2 сек либо обрабатываем по своему и меняем MAX, либо экстренно завершаем и пишем exit status = 14
        for(int i = 0;i < MAX;i++){
            sleep(1);
            printf("%d ", i);
        }
        printf("\nChild: завершил выполнение.\n");
    }
    return 0;
}

/*
Задание 9:
Через свой аларм меняем MAX, если же alarm не был переопределён, то экстренно завершаем дочерний.
*/