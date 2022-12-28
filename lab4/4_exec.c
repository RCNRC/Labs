#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char * argv[]){
    char buf[80];
    int n, l;
    printf("[SUB] Execve ok!\n");

    while((l=read(0, buf, 80))>0)
        write(1, buf, l);

    printf("[SUB] Terminate...\n");
}
