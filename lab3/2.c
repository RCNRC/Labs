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

int main(int argc, char * argv[], char * argp[]) {
    /*
    Использование:
    Запускается без параметров, ожидает сигнала SIGINT (ctrl+c).
    */
    struct sigaction new_act, old_act;
    printf("SA_ONSTACK = %d", SA_ONSTACK);
    new_act.sa_handler = signal_handler; // куда идёт обработка сигнала
    new_act.sa_flags = 0; // без дополнительных действий. можно было и SA_RESETHAND - восстановить по умолчанию
    sigaction(SIGINT, &new_act, &old_act);
    while(1){
        pause();
    }
    return 0;
}

/*
Задание 2:
Полностью аналогично предыдущему. Единствнное что - в данном случае бесполезный но требуемый old_act.
*/