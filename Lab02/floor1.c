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
#define bool int
#define true 1
#define false 0
/*
#define UP(x) (x*10+1)
#define DOWN(x) (x*10+2)
#define FLOOR(x) (x*10)
#define MSG_KEY 1234
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


*/
bool upNow = false;
bool downNow = false;
struct elevator_status* status;
sem_t* sem_id;

pthread_t main_tid;
void *monitor(void*arg){

	void *shmRead=NULL;
	int shmidRead;
	int*readerCnt;
	shmidRead=shmget((key_t)1001,4096,0666|IPC_CREAT);
	if(shmidRead==-1){
		printf("shared memory create failed %d\n",errno);
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	shmRead=shmat(shmidRead,(void*)0,0);
	
	if(shmRead==(void*)-1){
		printf("shmat failed %d",errno);
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	readerCnt=(int*)shmRead;
	sem_t*cntInit;
	cntInit=sem_open("cntInit",O_CREAT,0644,0);
	if(cntInit==SEM_FAILED){
		perror("unable to create semaphore");
		sem_unlink("cntInit");
		exit(-1);
	}
	*readerCnt=0;
	for(int i=0;i<NR_READER-1;i++)
		sem_post(cntInit);
	sem_t*mutex;
	mutex=sem_open("mutex",O_CREAT,0644,1);
	if(mutex==SEM_FAILED){
		perror("unable to create semaphore");
		sem_unlink("mutex");
		exit(-1);
	}
	sem_t*wrt;
	wrt=sem_open("wrt",O_CREAT,0644,1);
	if(wrt==SEM_FAILED){
		perror("unable to create semaphore");
		sem_unlink("wrt");
		exit(-1);
	}

	void *shm=NULL;
	int shmid;

	
	shmid=shmget((key_t)1000,4096,0666|IPC_CREAT);
	if(shmid==-1){
		printf("shared memory create failed %d\n",errno);
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	shm=shmat(shmid,(void*)0,0);
	if(shm==(void*)-1){
		printf("shmat failed %d",errno);
		perror("shmat");
		exit(EXIT_FAILURE);
	}
	status=(struct elevator_status*)shm;
	//status->direction=DIR_UP;
	//status->status=RUN;
	//status->floor=3;

	sem_t*statusInit;
	statusInit=sem_open("statusInit",O_CREAT,0644,0);
	if(statusInit==SEM_FAILED){
		perror("unable to create semaphore");
		sem_unlink("statusInit");
		exit(-1);
	}
	sem_wait(statusInit);
	
	while(1){
		sem_wait(mutex);
		(*readerCnt)++;
		if((*readerCnt)==1)
			sem_wait(wrt);
		sem_post(mutex);
		if(status->status==STOP&&status->floor==1){
			upNow=false;
		}
		if(status->status==STOP&&status->floor==1){
			downNow=false;
		}
		printf("The elevator is now at FLOOR %d ",status->floor);
		if(status->status==STOP){
			printf("and stopped\n");

		}
		else if(status->direction==DIR_UP){
			printf("and moving up\n");
		}
		else{
			printf("and moving down\n");
		}
		printf("You are at the 1st floor");
		if (upNow){
			printf(",and the UP button has been pressed");
		}
		if (downNow){
			printf(",and the DOWN button has been pressed");
		}
		printf("\nPlease choose the direction you want to go:\n");
		printf("1.UP\n");
		printf("Input your choice:\n");
    	//printf("%d %d %d\n",status->direction,status->status,status->floor);
		sem_wait(mutex);
		(*readerCnt)--;
		if((*readerCnt)==0)
			sem_post(wrt);
		sem_post(mutex);
		sleep(1);
	}
}
int main(){
	int err;
	err = pthread_create(&main_tid, NULL, monitor, NULL);
	if (err != 0){
		printf("Create thread error from 1st floor:");
		return 1;
	}
	int msgid = -1;
	msgid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
	if (msgid == -1){
		printf("msgget failed from 1st floor with error:%d\n", errno);
		exit(EXIT_FAILURE);
	}
	
    struct msg_st msgSend;
	msgSend.type = 1;
	while (1){
		/*printf("This is the 1st floor;\n");
		if (upNow){
			printf("UP button has been pressed\n");
		}
		if (downNow){
			printf("DOWN button has been pressed\n");
		}
		printf("Please choose the direction:\n");
		printf("1.UP\n");
		printf("2.DOWN\n");
		printf("Input your choice:\n");*/
		char choice[20];
		scanf("%s", choice);
		if (strlen(choice) == 1 && choice[0] == '1'){
			if (!upNow){
				msgSend.val = UP(1);
				if (msgsnd(msgid, (void*)&msgSend, 4, 0) == -1){
					printf("msgsnd failed from 1st floor\n");
					exit(EXIT_FAILURE);
				}
				upNow=true;
			}
		}
		/*
		else if (strlen(choice) == 1 && choice[0] == '2'){
			if (!downNow){
				msgSend.val = DOWN(1);
				if (msgsnd(msgid, (void*)&msgSend, 4, 0) == -1){
					printf("msgsnd failed from 1st floor\n");
					exit(EXIT_FAILURE);
				}
				downNow=true;

			}
		}
		*/
		else{
			printf("Input error. Please try again.\n");
		}
		
	}
}
