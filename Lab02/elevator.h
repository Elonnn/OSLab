#ifndef __EVEVATOR_
#define __EVEVATOR_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>                                                              
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/msg.h>  
#include <sys/shm.h> 
#include <sys/types.h>

#define UP(x) (x*10+1)
#define DOWN(x) (x*10+2)
#define FLOOR(x) (x*10)
#define MSG_KEY 1234
#define STATUS_KEY 1000
#define READERCNT_KEY 1001 
#define MUTEX_KEY "mutex" 
#define W_KEY "statusInit"
#define WRT_KEY "wrt" 
#define CLOSE_DOOR 1
#define OPEN_DOOR 0

#define CELLING 3
#define GROUND 1

#define NR_READER 4

enum DIRECTION {
	DIR_UP, DIR_DOWN 
};

enum STATUS{
	RUN, STOP
};

struct elevator_status {
	enum DIRECTION direction;
	enum STATUS status;
	int floor;
};

struct msg_st{
	long int type;
	int val;
};

#endif 
