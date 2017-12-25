// emsys.c
#include <stdio.h>
#include <stdlib.h>

#define _MEMSYS_CALL_ 392	//系统调用号

int main(int args, char* argc[]){
    if(args <= 1){
        printf("Params Error!");
    }
    int pid;
    sscanf(argc[1], "%d", &pid);
    printf("进程号: %d\n", pid);
    int tmp = syscall(_MEMSYS_CALL_, pid);
    return 0;
}