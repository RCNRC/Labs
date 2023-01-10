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

int main(int argc, char * argv[]){
    /*
    Использование: запсутить с 2 аргументами:
        1) Название файла очереди
        2) Тип сообщений для чтения
    */

    int msg_type = strtol(argv[2], NULL, 10);
    key_t key = ftok(argv[1], 1);
    printf("key = %d, type = %d\n", key, msg_type);
    int qd = msgget(key, IPC_CREAT | 0666);

    struct msg {
        long mtype;
        char mtext[4096];
    } msg;

    if(msgrcv(qd, &msg, 4096, msg_type, 0) == -1){
        perror("msgrcv");
        exit(0);
    }
    else{
        printf("Message: type = %ld, text = %s\n", msg.mtype, msg.mtext);
    }
    
    return 0;
}
