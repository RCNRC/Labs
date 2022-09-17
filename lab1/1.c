#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h> // for strerror

int main(){
    int errCode = open("unexist.txt", O_RDONLY); // trying to open unexisting file to get error code

    printf("Error code: %d \n", errCode); // error code after try
    printf("Errno: %d\n ", errno); // special var = code of last error
    perror("Perror function result");
    printf("Strerror(%d): %s\n", errno, strerror(errno)); // for gcc
    //printf("sys_errlist: %s\n", sys_errlist[errno]); // for old glibc

    /*
    for(int i = 0; i < 40; i++) {
        printf("sys_errlist[%d] = %s\n", i, strerror(i));
    }
    */

    return 0;
}
