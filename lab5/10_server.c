#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

key_t key;
int IPC_FLG = 0755;

void signal_handler(int sig){
    if(sig == SIGINT){
        int qd = msgget(key, IPC_FLG);
        msgctl(qd, IPC_RMID, NULL);
        exit(0);
    }
}

int main(int argc, char * argv[]){
    struct sigaction act, oact;
    act.sa_handler = signal_handler;
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &act, &oact);
   
    key = ftok(*argv, 1);
    int qd = msgget(key, IPC_CREAT | IPC_FLG);

    struct {
        long mtype;
        char mtext[4096];
    } msgr, msgw;

    printf("[Server] запущен.\nИспользуейте для запуска клиентов аргумент: %s\n", *argv);
    while(1){
        if(msgrcv(qd, &msgr, 4096, 0, 0) == -1)
            perror("msgrcv");
        else
            printf("[Server] Получено сообщение[ключ=%d]: %s\n", msgr.mtype, msgr.mtext);

        msgw.mtype = msgr.mtype;
        sprintf(msgw.mtext, "%d", strtol(msgr.mtext, NULL, 10) * strtol(msgr.mtext, NULL, 10));
        if(msgsnd(qd, &msgw, 4096, 0) == -1)
            perror("msgsnd");
        else
            printf("[Server] Ответ отправлен: key = %d, text = %s\n", msgw.mtype, msgw.mtext);
    }
}
