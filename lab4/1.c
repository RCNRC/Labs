#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char * argv []){
    /*
        fds[0] - чтение
        fds[1] - запись
    */
    int fds[2], fds_i[2];
    int pid;
    if(pipe(fds) == -1){
        perror("pipe error");
    }
    if(pipe(fds_i) == -1){
        perror("pipe error");
    }
    if((pid = fork()) == -1){
        perror("fork error");
    }

    if(pid){
        printf("[P] Дочерний процесс создан! PID = %d\n", pid);
        close(fds[0]);
        close(fds_i[1]);
        int num = 10;
        if(write(fds[1], &num, sizeof(int)) == -1){
            perror("[P] write error");
        }
        else{
            printf("\n[P] Пакет сгенерирован и отправлен. (VALUE = %d)\n", num);
        }

        if(read(fds_i[0], &num, sizeof(int)) == -1){
            perror("[P] read error!");
        }
        else{
            printf("[P] Получен ответ на пакет. VALUE = %d\n",num);
        }
        printf("[P] Завершение родительского процесса!\n");
        close(fds[0]);
        close(fds_i[1]);
        wait(NULL);
    }

    else{
        close(fds[1]);
        close(fds_i[0]);
        int another_num;
        if(read(fds[0], &another_num, sizeof(int)) == -1){
            perror("[C] read error");
        }
        else{
            printf("[C] Пакет принят! (VALUE = %d)\n", another_num);
            another_num *= 2;
            if(write(fds_i[1], &another_num, sizeof(int)) == -1){
                perror("[C] write error");
            }
            else{
                printf("[C] Ответ сформирован и отправлен! (VALUE = %d)\n", another_num);
            }
            printf("[C] Завершение дочернего процесса!\n");
            close(fds[1]);
            close(fds_i[0]);
        }
    }
}
/*
Задание 1.
В родителе создаём число, отправляем его сыну, тот его принимает, меняет и отправляет отцу. Отец принимает изменённое число, всё это печатается в терминале.
*/