#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

void logging_text(char text[]){
    int fh;
    if((fh = open("log.txt", O_RDWR | O_CREAT, 0755)) == -1)
        perror("open error");
    lseek(fh, 0L, SEEK_END);
    if(write(fh, text, strlen(text)) == -1)
        perror("write file");
    close(fh);
}

void signal_handler(int sig) {
    char s[4096];
    sprintf(s, "Handler: call by PID = %d\n", getpid());
    logging_text(s);
}

int * fibonacci(int fib_num_1, int fib_num_2, int N, int fh){
    int fib_num_3;
    int *result = malloc(2);
    char tmpbuf[30];
    int tmpbuf_buff;
    for(int i=0; i<N;i++){
        fib_num_3 = fib_num_1 + fib_num_2;
        tmpbuf_buff = sprintf(tmpbuf, "\n%d", fib_num_3);
        if(write(fh, tmpbuf, tmpbuf_buff) == -1)
            perror("write error");
        fib_num_1 = fib_num_2;
        fib_num_2 = fib_num_3;
    }

    result[0] = fib_num_2;
    result[1] = fib_num_3;
    return result;
}

int main(int argc, char * argv[]){
    /*
    Использование:
    Запустить программу без параметров.
    */
    int pid, fh;
    if((fh = open("log.txt", O_CREAT, 0755)) == -1)
        perror("creat error");
    close(fh);

    struct sigaction act, oact;
    act.sa_handler = signal_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, &oact);
    //sigaction(SIGINT, &act, &oact); // для удобного тестирования (ctrl+c)
    sigset_t mask, omask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, &omask);
    sigdelset(&mask, SIGUSR1);   // Сигнал, по которому синхронизируются процессы
    sigdelset(&mask, SIGINT);    // Для того, чтобы можно было выйти по ctrl+c

    int *fib_nums;
    char file_name[] = "Fibonacci.txt";
    if((fh = open(file_name, O_RDWR | O_CREAT, 0755)) == -1)
        perror("open error");
    if((pid = fork()) == -1){
        perror("fork error");
        return 0;
    }

    if(pid){ // parent
        logging_text("Parent: suspending myself\n");
        if(sigsuspend(&mask) == -1)
            perror("Parent: sigsuspend error");
        logging_text("Parent: after suspend, start reading file\n");
        char buff[1];
        int buff_l = 0L;
        do{
            lseek(fh, -1L, SEEK_CUR);
            buff_l += read(fh, &buff, 1);
            lseek(fh, -1L, SEEK_CUR);
            //printf("Parent: Char: %c\n", buff[0]);
        }while(buff[0]!='\n');
        char buff_m[buff_l];
        lseek(fh, 1L, SEEK_CUR);
        while(read(fh, buff, 1)>0){
            printf("%c", buff[0]);
        }
        printf("\n");
        logging_text("Parent: read last num\n");
        logging_text("Parent: try to SIGUSR1 the child\n");
        if(kill(pid, SIGUSR1) == -1)
            perror("Parent: kill error");
        logging_text("Parent: finish work\n");
        return 0;
    }
    else{ // child
        logging_text("Child: writing into the file\n");
        if(write(fh, "0\n1", 3) == -1)
            perror("Child: write error");
            
        fib_nums = fibonacci(0,1,10,fh);
        logging_text("Child: try to SIGUSR1 the parent\n");
        if(kill(getppid(), SIGUSR1) == -1)
            perror("Child: kill error");
        //sleep(1);
        logging_text("Child: suspending myself\n");
        if(sigsuspend(&mask) == -1)
            perror("Child: sigsuspend error");
        free(fib_nums);

        close(fh);
        logging_text("Child: finish work\n");
        return 0;
    }
}


/*
Задание 11.
Механизм, позволяющий потокам согласовывать свою работу с общими ресурсами - механизм синхронизации нитей (thread synchronization).

Вызов sigsuspend() временно заменяет маску сигналов вызывающего процесса маской, указанной в mask, и затем приостанавливает процесс до тех пор, пока не поступит сигнал, для которого запустится обработчик сигнала или это приведёт к завершению процесса.
Если по сигналу процесс завершается, то вызов sigsuspend() не возвращает управление обратно в программу. Если сигнал пойман, то возврат из sigsuspend() произойдёт после завершения выполнения обработчика, и маска сигналов восстановится в значение, которое было до вызова sigsuspend().
Сигналы SIGKILL и SIGSTOP невозможно заблокировать; указание этих этих сигналов в mask не влияет на маску сигналов процесса. 

Как работает программа: открываю для отца и сына файл хэндлер, задаю маску, блокирую все сигналы. Дальше форкаюсь, в отце жду(suspend) пока сын не запишет числа фибоначчи, в файл хэндлер. Сын даёт сигнал отцу SIGUSR1 и сам начинает ожидать. Отец читает из файл хэндлера последнее число, выводит его, посылает сыну сигнал SIGUSR1 и завершает свою работу. Сын пойав сигнал тоже завершает свою работу.

Вот что будет в логе работы "log.txt":
Parent: suspending myself
Child: writeng into the file
Child: try to SIGUSR1 the parent
Child: suspending myself
Handler: call by PID = 44969
Parent: after suspend, start reading file
Parent: read last num
Parent: try to SIGUSR1 the child
Parent: finish work
Handler: call by PID = 44970
Child: finish work
*/