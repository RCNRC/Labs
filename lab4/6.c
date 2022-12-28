#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[], char * argp[]){
    if(fork()){
        unlink("channel");
        mknod("channel",  S_IFIFO | 0755, 0);
        int wfd = open("channel",O_WRONLY);
        int i = 0;
        int n;
        int num = 10;
        printf("[TX] Client started...\n");
        while(i < 15){
            num = i;
            n = write(wfd, &num, sizeof(int));
            if(n < sizeof(int))
                printf("[TX] Ошибка при отправке пакета! (%d bytes)\n", n);
            else
                printf("[TX] Пакет отправлен! (%d) (%d bytes)\n", num, n);
            sleep(1);
            i++;
        }
    }
    else{
        int rfd = open("channel", O_RDONLY);
        int i = 0;
        int n;
        int other_num;
        sleep(3);
        printf("[RX] Server started...\n");
        while(i < 15){
            n = read(rfd, &other_num, sizeof(int));
            if(n < sizeof(int))
                printf("[RX] Ошибка при получении пакета! (%d bytes) \n", n);
            else
                printf("[RX] Пакет получен! (%d) (%d bytes)\n", other_num, n);
            sleep(1);
            i++;
        }
    }
}