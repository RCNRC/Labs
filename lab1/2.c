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
    
    По умолчанию нужно вводить chmod == 444 - только на чтение.
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
    int fh = open(argv[1], O_WRONLY | O_CREAT, chmod);
    if(fh == -1){
        printf("fh = -1, Open error 1.\n");
    }
    else{
        printf("file successfully opened 1.\n");
        strcpy(str, "First str\n");
        write(fh, str, strlen(str));
        strcpy(str, "Second str\n");
        write(fh, str, strlen(str));
    }
    close(fh);


    int scan;
    fh = open(argv[1], O_RDONLY);
    if(fh == -1){
        printf("fh = -1, Open error 2.\n");
    }
    else{
        printf("file successfully opened 2.\n");
        while(read(fh, str, 256) > 0){
            printf("%s", str);
        }
    }
    close(fh);


    fh = open(argv[1], O_RDWR);
    if(fh == -1){
        printf("fh = -1, Open error 3.\n");
    }
    else{
        printf("file successfully opened 3.\n");
    }
    close(fh);

    return 0;
}

/*
Задание 2: В первом опене мы создаём файл с правами только на чтение, однако сам файл при этом открываем этим же опеном на запись, из-за чего записываем в него. 
Второй опен на чтение пройдёт, а вот третий на запись уже нет.

Задание 3: Файл с правами только на чтение уже создан, поэтому первый опен и третий - не пройдут. Второй опен на чтение пройдёт.
*/