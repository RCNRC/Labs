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
#include <time.h>

int main(int argc, char * argv[]){

    int fd = open("queue_file", O_CREAT | O_TRUNC, 0777);

    key_t key = ftok("queue_file", 1);
    printf("key = %d\n", key);
    int qd = msgget(key, IPC_CREAT | 0666);
    
    struct msg {
        long mtype;
        char mtext[4096];
    } msg;

    char buff[4096];
    int scan;
    int i;
    for(i = 1; i < 5; i++){
        scan=read(0, buff, 4096);
        msg.mtype = (long)i;
        strcpy(msg.mtext, buff);
        msgsnd(qd, &msg, scan, IPC_NOWAIT);
    }
    
    struct msqid_ds qds;
    msgctl(qd, IPC_STAT, &qds);
    char str_t[100];
    printf("UID = %d\n", qds.msg_perm.uid);
    printf("GID = %d\n", qds.msg_perm.gid);
    printf("CUID = %d\n", qds.msg_perm.cuid);
    printf("CGID = %d\n", qds.msg_perm.cgid);
    strftime (str_t, 100, "%d.%m.%y %X", localtime(&qds.msg_stime));
    printf("Last msgsnd time = %s\n", str_t);
    strftime (str_t, 100, "%d.%m.%y %X", localtime(&qds.msg_rtime));
    printf("Last msgrcv time = %s\n", str_t);
    strftime (str_t, 100, "%d.%m.%y %X", localtime(&qds.msg_ctime));
    printf("Last change time = %s\n", str_t);
    printf("Max bytes = %ld\n", qds.msg_qbytes);
    printf("Number of messages: %ld\n", qds.msg_qnum);
    return 0;
}
