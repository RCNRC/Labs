#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>

void signal_handler(int sig) {
    printf("Сигнал %d получен.\n", sig);
    if(sig == SIGUSR1){
        printf("SIGUSR1 опознан, жду 15 секунд.\n");
        sleep(15);
    }
    printf("\n");
}

int main(int argc, char * argv[]) {
    /*
    Использование:
    Запустить программу без параметров.
    Дополнительно: после строчки "SIGUSR1 опознан, жду 15 секунд." можно понажимать (ctrl+c = SIGINT)
    */
    struct sigaction new_act, old_act;
    sigset_t new_set, old_set;

    new_act.sa_handler = signal_handler;
    if(sigaction(SIGUSR1, &new_act, &old_act) == -1)
        perror("sigaction error");
    if(sigaction(SIGUSR2, &new_act, &old_act) == -1)
        perror("sigaction error");
    if(sigaction(SIGCONT, &new_act, &old_act) == -1)
        perror("sigaction error");
    if(sigaction(SIGINT, &new_act, &old_act) == -1)
        perror("sigaction error");
    if(sigfillset(&new_set) == -1) // наполняю набор сигналами, на который указывает new_set
        perror("sigemptyset error");
    if(sigdelset(&new_set, SIGUSR1) == -1) // из этого набора убираю сгнал SIGUSR1
        perror("sigdelset error");
    sigprocmask(SIG_SETMASK, &new_set, &old_set); // блокирую все сигналы (кроме SIGUSR1)
    int pid;
    kill(getpid(), SIGUSR1); // сигнал SIGUSR1 не заблокирован, значит мы его можем обработать
    kill(getpid(), SIGINT);
    kill(getpid(), SIGINT);
    kill(getpid(), SIGINT);
    kill(getpid(), SIGCONT);
    kill(getpid(), SIGCONT);
    kill(getpid(), SIGCONT);
    kill(getpid(), SIGUSR2);
    kill(getpid(), SIGUSR2);
    kill(getpid(), SIGUSR2);
    
    sigprocmask(SIG_SETMASK,&old_set,0);
}
/*
Задание 10:
Для удобства: основной сигнал - SIGUSR1, дополнительные: SIGUSR2, SIGINT, SIGCONT.
Все эти 4 сигнала устанавливаем на пройстейший обработчик, который лишь выводт номер полученного сигнала. Для основного сигнала он также устанавливает 15 секунд простоя.
Создаём набор со всеми сигналами кроме SIGUSR1 и блокируем их.
За отведённые 15 секунд можно понажимать (ctrl+c = SIGINT).
Затем программа сама посылает множество дополнительных сигналов.
После того как снимется блокировка, все заблокированные сигналы будут обработаны по одному разу (по убыванию номера сигнала).

Из https://www.opennet.ru/base/dev/unix_signals.txt.html:
Если сигнал блокируется, то его обработчик будет вызван один раз (независимо от того, сколько раз был послан сигнал) после снятия блокировки.
*/