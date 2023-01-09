#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char * argv[]){
    struct stat st;
    int code = stat(argv[1], &st);
    char str_t[100];

    printf("Устройство: %d\n", st.st_dev);
    printf("inode: %d\n", st.st_ino);
    printf("Режим доступа: %d\n", st.st_mode);
    printf("Кол-во жестких ссылок: %d\n", st.st_nlink);
    printf("ID пользователя-владельца: %d\n", st.st_uid);
    printf("ID группы-владельца: %d\n", st.st_gid);
    printf("Тип устройства: %d\n", st.st_rdev);
    printf("Общий размер в байтах: %d\n", st.st_size);
    printf("Размер блока ввода-вывода: %d\n", st.st_blksize);
    printf("Количество выделенных блоков: %d\n", st.st_blocks);
    strftime (str_t, 100, "%d.%m.%y %X", localtime(&st.st_atime));
    printf("Время последнего доступа: %s\n", str_t);
    strftime (str_t, 100, "%d.%m.%y %X", localtime(&st.st_mtime));
    printf("Время последней модификации: %s\n", str_t);
    strftime (str_t, 100, "%d.%m.%y %X", localtime(&st.st_ctime));
    printf("Время последнего изменения: %s\n", str_t);

    printf("\n");
    int pid = getpid();
    printf("ID текущего процесса: %d\n", pid);
    int pgid = getpgid(pid);
    printf("ID группы текущего процесса: %d\n", pgid);
    int sid = getsid(pid);
    printf("ID сессии текущего процесса: %d\n", sid);    

    return 0;
}
