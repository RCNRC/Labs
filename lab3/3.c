#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void print_process_id(char who[]){
    printf("%s: PID: %d, PPID: %d, process group: %d\n", who, getpid(), getppid(), getpgrp());
}

void no_zombee(){
    struct sigaction act, oact;
    act.sa_handler = SIG_DFL;
    act.sa_flags = SA_NOCLDWAIT;
    if (sigaction(SIGCHLD, &act, &oact) == -1){
        perror("Sigaction error");
    }
}

int main(int argc, char * argv[], char * argp[]){
    /*
    Использование:
    Запустить программу без параметров и ждать завершения родительского процесса.
    */
   
    no_zombee();  // закомментировать, чтобы увидеть зомби <defunct> в выводе ps.

    int pid;
    if((pid=fork()) == -1){
        perror("Fork error");
        return -1;
    }

    if(pid){
        print_process_id("Parent");
        printf("Parent: fork returned child pid = %d\n", pid);
        //waitpid(pid);  // вот второй способ.
        sleep(1);
        system("ps");
    }
    else{  // pid = 0
        print_process_id("Child");
    }

    return 0;
}

/*
Задание 3:
При завершении дочернего процесса (освобождаются все ресурсы кроме PID), тот отправляет сигнал SIGCHLD родительскому процессу. 
В свою очередь родительский процесс, хоть и ловит сигнал, но никак его не обрабатывает сам. (Кстати, зомби нельзя убить через kill, т.к. зомби не принимает сигналов).
Оставшийся PID (заблокированная запись в таблице процессов) от дочернего освобождается только после обработки сигнала SIGCHLD в родительском процессе.

Вариант автоматической обработки SIGCHLD в родительском процессе только один: завершить родительский процесс.

Варианты ручной обработки SIGCHLD в родительском процессе:
    1. Вручную назначить PPID = 1 - установить родителем инициализирующий процесс init, у которого PID = 1. Пока не знаю как.
    2. Вручную добавить в родителе к дефолтной обработке SIGCHLD флаг SA_NOCLDWAIT, что означает автоматическое дозавершение дочерних процессов. (no_zombee()).
    3. Вручную где-либо в родителе ждать завершения дочернего с помощью команд wait() или waitpid().
*/