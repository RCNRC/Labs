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
    /*
    Использование:
    В параметрах указать названия файлов, среди которых нужно найти самый большой по размеру.
    */
    struct stat st;
    int max_size = -1, index = 0; // файл может быть нулевым по размеру
    for(int i = 1; i < argc; i++){
        if(stat(argv[i], &st)==0)
            if(S_IFREG) // проверка на обычный файл
                if(st.st_size>max_size){
                    max_size = st.st_size;
                    index = i;
                }
        else{
            printf("%s - not a usual file.\n", argv[i]);
        }
    }
    if(max_size!=-1)
        printf("File %s has max size = %dB\n", argv[index], max_size);
    else
        printf("No files.\n");

    return 0;
}
