#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char * argv[]){
    /*
    Использование:
    1. При отсутствии аргументов из командной строки банально копирует весь ввод в вывод.
    2. При одном аргументе выводит содержимое файло подобно cat.
    3. При двух аргументах - выводит содержимое из первого аргумента во второй файл.
    */
    int fh_in=0, fh_out=1, scan;
    char buff[4096];
    if(argc>3)
        return -1;
    if(argc>2)  // если 2 аргумента - открыть второй на копирование
        fh_out = creat(argv[2],  0777);
    if(argc>1)  // если минимум один аргумент, то открываем первый на чтение
        fh_in = open(argv[1], O_RDONLY);
    // если аргументов нет, fh остануться со стандартными значениями на вход и на выход.

    while((scan=read(fh_in, buff, 4096))>0)
        write(fh_out, buff, scan);

    close(fh_in);
    close(fh_out);

    return 0;
}
