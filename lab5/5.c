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
        1) нужно найти очередь по файлу (=f) или по key (=k)
        2) имя файла или key
    */

    key_t key;
    if(strcmp(argv[1], "f"))
        if((key = ftok(argv[2], 1)) == -1)
            perror("ftok");
    else{
        if(strcmp(argv[1], "k"))
            key = strtol(argv[2], NULL, 10);
        else{
            printf("Unknown option.\n");
            return -1;
        }
    }

    int qid;
    if((qid = msgget(key, 0666)) == -1){
        perror("msgget");
    }
    if(msgctl(qid, IPC_RMID, NULL) == -1){
        perror("msgctl");
        exit(1);
    }
    printf("Queue with key %d was deleted.\n", key);
    return 0;
}
