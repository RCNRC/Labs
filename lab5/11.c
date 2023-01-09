#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <math.h>

key_t key;
int IPC_FLG = 0755;

void free_queue(int sig){
    if(sig == SIGINT){
        int qd = msgget(key, IPC_FLG);
        msgctl(qd, IPC_RMID, NULL);
        exit(0);
    }
}

int main(int argc, char * argv[]){
    /*
    gcc 11.c -o 11 -lm

    Использование: запустить с 2 аргументами:
        1) число дочерних процессов,
        2) число, до которого берётся интервал случайных чисел. (интервал берётся от 0)
    */

    struct sigaction act, oact;
    act.sa_handler = free_queue;
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &act, &oact);
   
    key = ftok(*argv, 1);
    int qd = msgget(key, IPC_CREAT | IPC_FLG);

    struct {
        long mtype;
        char mtext[4096];
    } msgr, msgw;

    int pre_res = 0;
    int proc_num = strtol(*(argv+1), NULL, 10);

    if(fork()){
        printf("[Server] запущен.\n");
        int j1;
        for(j1 = 0; j1 < proc_num; j1++){
            if(msgrcv(qd, &msgr, 4096, 0, 0) == -1)
                perror("msgrcv");
            pre_res += strtol(msgr.mtext, NULL, 10);
        }
        printf("Среднеквадратическое = %f\n", sqrt((double)pre_res/(double)proc_num));
        free_queue(SIGINT);
    }
    else{
        time_t t;
        srand((unsigned) time(&t));
        int max_num = strtol(*(argv+2), NULL, 10);
        int num;
        int j;
        int nums[proc_num];
        for(j = 0; j < proc_num; j++)
            nums[j] = rand()%max_num;
        for(j = 0; j < proc_num; j++){
            if(fork()==0){ //child-child
                num = nums[j];
                msgw.mtype = getpid();
                sprintf(msgw.mtext, "%d", num * num);
                msgsnd(qd, &msgw, 4096, 0);
                exit(0);
            }
            else{;} //child-parent
        }
        exit(0);
    }
}

/*
Задание 11:
Простейшая реализация распоралелливания вычислительной задачи: подсчёт среднего квадратического числа.
Дочерние процессы вычисляют x^2 и передают серверу.
Сервер считывает x^2 от дочерних процессов и суммирует их себе в переменную pre_res (= сумма квадратов). 
Когда все дочерние процессы отработают, сервер выдаст ответ = sqrt(pre_res/proc_num) = среднее квадратическое.
*/