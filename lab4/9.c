#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[]){
    /*
    Использование:
    запстить с параметром - названием файла.
    */
    if(argc < 2){
        printf("Usage: %s filename", argv[0]);
        exit(1);
    }
    int pid;
    if((pid = fork()) == -1){
        perror("fork error");
        exit(1);
    }

    if(pid){
        int fd;
        if((fd = open(argv[1], O_WRONLY | O_CREAT, 0755)) == -1){
            perror("[P] open error");
            exit(1);
        }
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        fcntl(fd, F_SETLKW, lock);
        int i = 0;
        int val;
        char c[7];
        do{
            sprintf(c, "%d, ", i);
            if(write(fd, &c, sizeof(c)) == -1){
                
                perror("[P] write error");
            }
        } while(i++ < 100);
        fcntl(fd, F_SETLKW, lock);
        close(fd);
    }
    else{
        int fd;
        if((fd = open(argv[1], O_RDONLY)) == -1){
            perror("[P] open error");
            exit(1);
        }
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_start = 0;
        lock.l_whence = SEEK_SET;
        lock.l_len = 0;
        fcntl(fd, F_SETLKW, lock);
        int i = 0;
        int val;
        char *c;
        int k;
        do{
            k = read(fd, &c, 1);
            if( k == -1){
                perror("[P] write error");
            }
            write(1, &c, 1);
        } while(k!=0);
        fcntl(fd, F_SETLKW, lock);
        printf("\n");
        close(fd);

    }
    return 0;
}
/*

*/