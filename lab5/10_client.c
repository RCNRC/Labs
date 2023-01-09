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

int main(int argc, char * argv[]){
    key = ftok(*(argv+1), 1);
    int qd = msgget(key, 0);

    if(qd == -1){
        printf("[Client] сервер не открыл очередь, завершаю работу.\n");
        exit(0);
    }

    struct {
        long mtype;
        char mtext[4096];
    } msgr, msgw;

    printf("[Client] запущен.\n");
    while(1){
        printf("[Client] Введите число:\n");
        scanf("%s", msgw.mtext);
        msgw.mtype = getpid();
        if(msgsnd(qd, &msgw, 4096, 0) == -1){
            if(msgget(key, 0) == -1){
                printf("[Client] сервер закрыл очередь, завершаю работу.\n");
                exit(0);
            }
        }
        else
            printf("[Client] Сообщение отправлено.\n");

        if(msgrcv(qd, &msgr, 4096, getpid(), 0) == -1)
            perror("msgrcv");
        else
            printf("[Client] Получен ответ с ключом %d: %s\n", msgr.mtype, msgr.mtext);
    }
}
