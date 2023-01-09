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
    int qd = msgget(key, IPC_CREAT | 0666);

    struct msg {
        long mtype;
        char mtext[4096];
    } msg;

    msgrcv(qd, &msg, 4096, msg_type, IPC_NOWAIT | MSG_NOERROR);
    printf("Message: type = %d, text = %s\n", msg.mtype, msg.mtext);
    if(msg.mtype!=msg_type){
        //printf("Message: type = %d, text = %s.\nThis is bullshit, exit(-1).\n", msg.mtype, msg.mtext);
        exit(-1);
    }
    
    return 0;
}
