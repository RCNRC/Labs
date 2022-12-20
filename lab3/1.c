#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void signal_handler(int signal_id){
    printf("Сигнал SIGINT перехвачен.\n");
    signal(SIGINT, SIG_DFL);
    printf("Сигналу SIGINT возвращено дефолтное поведение.\n");
}

int main(int argc, char * argv[], char * argp[]){
    /*
    Использование:
    Запускается без параметров, ожидает сигнала SIGINT (ctrl+c).
    */
    signal(SIGINT, signal_handler);
    while(1){
        pause();
    }
    return 0;
}

/*
Задание 1:
При вызове ждёт в бесконечном while сигнала SIGINT (ctrl+c).
После получения первого такого сигнала, программу переведёт его обработку в функцию signal_handler, где ему зададут дефолтное поведение.
Соответственно, при повторном вызове сигнал будет обработан уже дефолтным образом, т.е. закроет программу.
(Если бы в функции signal_handler был SIG_IGN, то мы бы тогда задали ему поведение игнорирования этого сигнала).
*/