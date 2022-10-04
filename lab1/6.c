#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char * argv[]){
    /*
    Использование: 
    в первом аргументе - любое имя для файла с расширением;
    во втором аргументе - трёхзначное восьмиричное число chmod.
    
    По умолчанию нужно вводить chmod == 666 - на чтение и на запись.
    Права доступа = (chmod) && (not umask).
    */

    if(argc == 1){
        printf("No arguments: filename, permissions\n");
        return -1;
    }
    else if(argc == 2){
        printf("No argument: permissions\n");
        return -1;
    }


    int chmod = strtol(argv[2], 0, 8);
    char str[255];
    int fh = open(argv[1], O_RDWR | O_CREAT, chmod);
    if(fh == -1){
        printf("fh = -1, Open error. Very strange.\n");
    }
    else{
        strcpy(str, "1");
        write(fh, str, strlen(str));
        lseek(fh, 10000, SEEK_CUR); // идём от текущей позиции на 4 символов вправо
        strcpy(str, "1");
        write(fh, str, strlen(str));
    }
    close(fh);

    return 0;
}

/*
Задание 6:
Создание разреженного файла - это создание файла с дырой.
*/