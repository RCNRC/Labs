#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
int main(int argc, char * argv[]){

    int pid;
    if((pid = fork()) == -1){
        perror("fork error");
        exit(1);
    }

    char buf[100];
    if(pid){
        int n;
        while(1){
           if((n = read(0, &buf, sizeof(buf))) == -1){
                perror("[P] read error");
            }
            write(1, "[P] ", 4);
            if(write(1, buf, n) == -1){
                perror("[P] write error");
            }
        }
    }
    else{
        printf("[C] Child process started. PID = %d\n", getpid());
        int n;
        while(1){
            if((n = read(0, &buf, sizeof(buf))) == -1){
                perror("read error");
            }
            write(1, "[C] ", 4);
            if(write(1, buf, n) == -1){
                perror("write error");
            }
        }
    }
    return 0;
}

/*
Что будет: при вводе строки её выводит либо отец, либо сын.
*/