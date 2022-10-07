#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char * argv[]){
    /*
    Использование:
    Единственный необходимый аргумент - название файла, который будет выведен задом наперёд.
    */
    int fh;
    if((fh = open(argv[1], O_RDONLY)) == -1){
        printf("Open error.\n");
        return -1;
    }

    char buff;
    lseek(fh, 1, SEEK_END);

    int l=1;
    while(l!=-1){
        read(fh, &buff, 1);
        write(1, &buff, 1);
        l = lseek(fh, (long)-2, SEEK_CUR);
    }
    
    printf("\n");
    close(fh);

    return 0;
}
