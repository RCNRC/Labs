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
        strcpy(str, "0");
        write(fh, str, strlen(str));
        lseek(fh, 4, SEEK_CUR); // идём от текущей позиции на 4 символов вправо
        strcpy(str, "cur");
        write(fh, str, strlen(str));
        lseek(fh, 3, SEEK_SET); // идём на 3-ий символ
        strcpy(str, "set");
        write(fh, str, strlen(str));
        lseek(fh, 2, SEEK_END); // идём на 2 символа вправо от конца файла
        strcpy(str, "end");
        write(fh, str, strlen(str));
        lseek(fh, 0, SEEK_SET); // идём на 0-ой символ
        read(fh, str, 256);
        printf("string:%s\n",str);
    }
    close(fh);

    return 0;
}

/*
Задание 4:
Если запустить скрипт, то в созданном файле появится много дыр, которые можно увидеть через редактор, но нельзя через cat, он заполнит дыры массивом нулей.
Посмотреть сколько занимает файл места можно через команду ls -l file.txt, а де факто число реально занимаемого места можно посмотреть через команду du -h file.txt

Задание 5:
Если выполнить повторно программу с тем же файлом, то будет добавлено больше дыр.
*/