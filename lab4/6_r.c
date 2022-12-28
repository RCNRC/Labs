#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[], char * argp[]){
    //unlink("channel");
    int rfd = open("channel", O_RDONLY);
    int i = 0;
    int n;
    int other_num;
    sleep(5);
    printf("[RX] Server started...\n");
    while(i < 15){
        n = read(rfd, &other_num, sizeof(int));
        if(n < sizeof(int))
            printf("[RX] Ошибка при получении пакета! (%d bytes) \n", n);
        else
            printf("[RX] Пакет получен! (%d) (%d bytes)\n", other_num, n);
        sleep(1);
    }
}
