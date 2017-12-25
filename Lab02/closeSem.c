#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/msg.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"elevator.h"
int main(){
sem_t*cntInit;
cntInit=sem_open("cntInit",O_CREAT,0644,0);
if(cntInit==SEM_FAILED){
	perror("unable to create semaphore");
	sem_unlink("cntInit");
	exit(-1);
}
sem_close(cntInit);
sem_unlink("cntInit");

sem_t*mutex;
mutex=sem_open("mutex",O_CREAT,0644,1);
if(mutex==SEM_FAILED){
	perror("unable to create semaphore");
	sem_unlink("mutex");
	exit(-1);
}
sem_close(mutex);
sem_unlink("mutex");

sem_t*wrt;
wrt=sem_open("wrt",O_CREAT,0644,1);
if(wrt==SEM_FAILED){
	perror("unable to create semaphore");
	sem_unlink("wrt");
	exit(-1);
}
sem_close(wrt);
sem_unlink("wrt");
return 0;
}
