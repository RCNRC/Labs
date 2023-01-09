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

key_t key_read;
key_t key_write;
int IPC_FLG = 0755;

int main(int argc, char * argv[]){
    key_read = ftok(*(argv+1), 2);
    key_write = ftok(*(argv+1), 1);
    int qdr = msgget(key_read, 0);
    int qdw = msgget(key_write, 0);

    if(qdw == -1){
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
        if(msgsnd(qdw, &msgw, 4096, 0) == -1){
            if(msgget(key_write, 0) == -1){
                printf("[Client] сервер закрыл очередь, завершаю работу.\n");
                exit(0);
            }
        }
        else
            printf("[Client] Сообщение отправлено.\n");

        if(msgrcv(qdr, &msgr, 4096, getpid(), 0) == -1)
            perror("msgrcv");
        else
            printf("[Client] Получен ответ с ключом %d: %s\n", msgr.mtype, msgr.mtext);
    }
}
