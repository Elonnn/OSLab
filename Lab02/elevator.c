#include "elevator.h"
#include <pthread.h>

struct elevator_status status = { DIR_UP,STOP,1 };

struct elevator_status* shared_memory;
sem_t* sem_id;
sem_t* w;
sem_t* wrt;

/* in thread */
pthread_mutex_t mutex;
int up[4] = { 0,0,0,0};
int down[4] = { 0,0,0,0};
int dst[4] = { 0,0,0,0};

void writeIntoStatus() {
	sem_wait(wrt);
	memcpy(shared_memory, &status, sizeof(status));
	sem_post(wrt);
}

void* dealWithMessage() {
	/* Initial the message queue */
	int msgid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
	if (msgid == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	/* receive the messages */
	struct msg_st data;
	int msg_type = 0;
	while (1) {
		if (msgrcv(msgid, (void*)&data, sizeof(int), msg_type, 0) == -1) {
			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
			exit(EXIT_FAILURE);
		}

		/*for test*/
		//printf("%d\n", data.val);

		pthread_mutex_lock(&mutex);
		int count = 0;
		for (int i = GROUND; i<=CELLING; i++) {
			count += (up[i] + down[i] + dst[i]);
		}
		if (data.val == OPEN_DOOR && status.status == STOP) {
			dst[status.floor] = 1;
		}
		else if (data.val != OPEN_DOOR && data.val != CLOSE_DOOR) {
			int dstfloor = data.val / 10;
			if (data.val == UP(dstfloor))
				up[dstfloor] = 1;
			else if (data.val == DOWN(dstfloor))
				down[dstfloor] = 1;
			else
				dst[dstfloor] = 1;
			if (count == 0 && dstfloor != status.floor) {
				status.direction = dstfloor > status.floor ? DIR_UP : DIR_DOWN;
				writeIntoStatus();
			}
		}
		pthread_mutex_unlock(&mutex);
	}


	if (msgctl(msgid, IPC_RMID, 0) == -1) {
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
}
void* dealWithElevator()
{
	while (1)
	{

		int flag_up = 0;
		int flag_down = 0;
		int flag_cur = 0;
		int step = 0;

		pthread_mutex_lock(&mutex);
		
		writeIntoStatus();

		int curfloor = status.floor;

		flag_cur = up[curfloor] + down[curfloor] + dst[curfloor];

		for(int i=curfloor+1;i<=CELLING;i++){
			flag_up += (up[i] + down[i] + dst[i]);
		}
		for(int i=curfloor-1;i>=GROUND;i--){
			flag_down += (up[i] + down[i] + dst[i]);
		}

		pthread_mutex_unlock(&mutex);

		/* wait here */
		if(flag_up + flag_down + flag_cur == 0)
			continue;

		/* need to run */
		/* whether to stop */
		pthread_mutex_lock(&mutex);

		int toStop = dst[curfloor];

		dst[curfloor] = 0;
		if(status.direction == DIR_UP){
			toStop |= up[curfloor];
			toStop |= (down[curfloor] & !flag_up);
			up[curfloor] = 0;
			//down[curfloor] = flag_up ? down[curfloor] : 0;
		}
		else{
			toStop |= down[curfloor];
			toStop |= (up[curfloor] & !flag_down);
			down[curfloor] = 0;
			//up[curfloor] = flag_down ? up[curfloor] : 0;
		}

		if(toStop)
			status.status = STOP;

		pthread_mutex_unlock(&mutex);
		
		printf("At floor %d\n",curfloor);
		sleep(2);

		if(toStop){
			pthread_mutex_lock(&mutex);
			writeIntoStatus();
			pthread_mutex_unlock(&mutex);
			printf("open the door\n");
			sleep(2);
			printf("close the door\n");
			sleep(2);
		}

		/* continue to run */
		pthread_mutex_lock(&mutex);

		status.status = RUN;

		if(!toStop)
			status.floor += status.direction == DIR_UP ? 1 : -1;
		else{
			if(status.direction == DIR_UP)
				status.direction = flag_up ? DIR_UP : DIR_DOWN;
			else
				status.direction = flag_down ? DIR_DOWN : DIR_UP;		
		
			if(curfloor == GROUND)
				status.direction = DIR_UP;
			if(curfloor == CELLING)
				status.direction = DIR_DOWN;
		
			if(dst[curfloor] || (up[curfloor] && status.direction==DIR_UP)
				|| (down[curfloor] && status.direction==DIR_DOWN)){
				step = 0;
				if(status.direction==DIR_UP)
					up[curfloor] = 0;
				else
					down[curfloor] = 0;
			}
			else
				step = status.direction == DIR_UP ? !!flag_up : -(!!flag_down);

			status.floor += step;
			if(flag_up + flag_down + up[curfloor] + down[curfloor] + dst[curfloor] == 0)
				status.status = STOP;
		}
		pthread_mutex_unlock(&mutex);

	}


}

int main(int argc, char* argv[]) {
	/* Map the shared memory for status */
	int shmid_status = shmget((key_t)STATUS_KEY, 4096, 0666 | IPC_CREAT);
	if (shmid_status == -1) {
		printf("shared memory create failed %d\n", errno);
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	void* shm_status = shmat(shmid_status, (void*)NULL, 0);
	if (shm_status == (void*)-1) {
		printf("shmat failed %d", errno);
		perror("shmat");
		exit(EXIT_FAILURE);
	}
	shared_memory = (struct elevator_status*) shm_status;

	/* semaphore for writer*/
	wrt = sem_open(WRT_KEY, O_CREAT, 0644, 1);
	if (wrt == SEM_FAILED) {
		perror("unable to create semaphore");
		sem_unlink("wrt");
		exit(-1);
	}

	/* for initial */
	w = sem_open(W_KEY, O_CREAT, 0644, 0);
	if (w == SEM_FAILED) {
		perror("unable to create semaphore");
		sem_unlink(W_KEY);
		exit(-1);
	}

	/* Initialize the shared memory */
	writeIntoStatus();
	/* increase 4 times because 4 process need to wait */
	for (int i = 0; i<NR_READER; i++)
		sem_post(w);

	pthread_t mDealer;

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&mDealer, NULL, dealWithMessage, NULL);
	/* TODO add your pthread here */

	pthread_t mElevator;

	pthread_create(&mElevator, NULL, dealWithElevator, NULL);

	/* TODO add join */
	pthread_join(mDealer, NULL);
	pthread_join(mElevator, NULL);
	pthread_mutex_destroy(&mutex);

	/* close the sem */
	sem_close(w);
	sem_close(wrt);
	sem_unlink(W_KEY);
	sem_unlink(WRT_KEY);

	/* close the shared memory */
	if (shmdt(shm_status) == -1) {
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	if (shmctl(shmid_status, IPC_RMID, 0) == -1) {
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
