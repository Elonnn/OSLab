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
	sem_wait(cntInit);

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
		printf("This is inside the elevator;\n");
		printf("Please choose the destination or open/close the door:\n");
		printf("1.FLOOR 1\n");
		printf("2.FLOOR 2\n");
		printf("3.FLOOR 3\n");
		printf("4.OPEN THE DOOR\n");
		printf("5.CLOSE THE DOOR\n");
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
	bool upNow = false;
	bool downNow = false;
    struct msg_st msgSend;
	msgSend.type = 1;
	while (1){
		/*printf("This is inside the elevator;\n");
		printf("Please choose the destination or open/close the door:\n");
		printf("1.FLOOR 1\n");
		printf("2.FLOOR 2\n");
		printf("3.FLOOR 3\n");
		printf("4.OPEN THE DOOR\n");
		printf("5.CLOSE THE DOOR\n");
		printf("Input your choice:\n");*/
		char choice[20];
		scanf("%s", choice);
		if (strlen(choice) == 1 && choice[0] == '1'){
				msgSend.val = FLOOR(1);
				if (msgsnd(msgid, (void*)&msgSend, 4, 0) == -1){
					printf("msgsnd failed from 1st floor\n");
					exit(EXIT_FAILURE);
				}
		}
		else if (strlen(choice) == 1 && choice[0] == '2'){
			msgSend.val = FLOOR(2);
				if (msgsnd(msgid, (void*)&msgSend, 4, 0) == -1){
					printf("msgsnd failed from 1st floor\n");
					exit(EXIT_FAILURE);
				}
		}
		else if (strlen(choice) == 1 && choice[0] == '3'){
			msgSend.val = FLOOR(3);
				if (msgsnd(msgid, (void*)&msgSend, 4, 0) == -1){
					printf("msgsnd failed from 1st floor\n");
					exit(EXIT_FAILURE);
				}
		}
		else if (strlen(choice) == 1 && choice[0] == '4'){
			msgSend.val = OPEN_DOOR;
				if (msgsnd(msgid, (void*)&msgSend, 4, 0) == -1){
					printf("msgsnd failed from 1st floor\n");
					exit(EXIT_FAILURE);
				}
		}
		else if (strlen(choice) == 1 && choice[0] == '5'){
			msgSend.val = CLOSE_DOOR;
				if (msgsnd(msgid, (void*)&msgSend, 4, 0) == -1){
					printf("msgsnd failed from 1st floor\n");
					exit(EXIT_FAILURE);
				}
		}
		else{
			printf("Input error. Please try again.\n");
		}
		
	}
}
