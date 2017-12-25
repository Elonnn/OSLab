#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>


#define WORK_SIZE 1024
#define MAX_COMMANDS 6
char work_area[WORK_SIZE];	//�������
int cmds_buffer[MAX_COMMANDS];	// 12,13,21,23,31,32 ע��12��ʾ���û�ϣ����1¥ǰ��2¥����������
int cmds_processing[MAX_COMMANDS]; //���ڴ��������
int cmd_bptr = 0; //buffer ptr
int cmd_pptr = 0; //processing ptr
Elevator elevator;
int print_flags;
pthread_mutex_t work_mutex; //������


enum State{
    up,down,stop,open,close
};//����״̬ ����\����\ֹͣ,����
enum gateState{
    open,close
};//������ ��\�� 
struct Elevator{
    int floor;//��ǰ¥�� 
    State state;//����״̬ 
    gateState gate;//��״̬ 
}; 

void init_elevator();//���ݳ�ʼ�� 
void show_state();//��ʾ����״̬ 
void ele_up(int des_flr);//���� 
void ele_down(int des_flr);//���� 

void *ele_monitor();//��������ʾ���
void *ele_control();//�����п������ 
void *flr_monitor();//ÿ��¥����ʾ��� 
void *flr_control();//ÿ��¥������� 


//���ݴ���ֹͣ״̬ʱ����������ɨ��ȴ��������Ӧ���飬Ȼ������decision������������������
State decision(void){	//�������ݿ����߼���i.e. ���У����У�ֹͣ
	// ���ݿ����߼�������cmds_buffer[MAX_COMMANDS]��cmds_processing[MAX_COMMANDS],�Լ���������״̬����

}


int main(int argc,char *argv[])
{
    /*�ο�˼·
	  ������ģ��������У�����ı�¥�㣬�����ĸ��ӽ��̷ֱ�ģ��1,2,3¥�Ŀ�����弰�����ڿ�����壬����I/O��

	  ��һ�� 1,2,3¥�Ŀ�����������������2���̣߳��ȴ��û�����������ô��ȡ���룿
	  		��1,2,3,¥�Ŀ����߳��ڴ���һ��linux�նˣ�ÿ���ն˿�������Ϊfloor X control panel����Զ�ȴ��û�����,�����Դ�ȷ��

	  �ڶ��� �����ڵĿ����������е��߳̿ɸ�����Ҫ������������ʱ����������ʱ������Ҳ��һֱ����

	  ����������һ���㹻���Ļ�����ʵ��������û�����(Ŀ��¥��1,2,3���û�����¥��)��һ��������ʵ����鳤��ָ��
	  		��дһ��decision(int current_floor)����ģ����ݿ����߼���ɨ��������������������з�ʽ
	  		������ѭ������������飬����decision������
	*/ 
	
	pid_t floor_1,floor_2,floor_3,in_ele,elevator;//�����ĸ��ӽ��� 
    pid_t child;
    pthread_t monitor,control;//�̣߳���ʾ�����ڲ�����״̬��������� 
    int error;


    floor_1 = fork();
    if(floor_1<0) perror("create process 1 error!\n");
    else if(floor_1==0){	//child process 1
    	pthread_t control_panel,display_panel;
    	int error;

    	error = pthread_create(&control_panel,NULL/*thread attr*/,flr_control,NULL/*args*/);
		if(error!=0){
			perror("create user thread failed!\n");
			return -1;
		}
		else{
			printf("prthread create is created!\n");
		}

		error = pthread_create(&display_panel,NULL/*thread attr*/,flr_monitor,NULL/*args*/);
		if(error!=0){
			perror("create user thread failed!\n");
			return -1;
		}
		else{
			printf("prthread create is created!\n");
		}

		error = pthread_join(control_panel,NULL);
		if(error!=0){
			perror("pthread join failed!\n");
		}
		error = pthread_join(display_panel,NULL);
		if(error!=0){
			perror("pthread join failed!\n");
		}
    }

    floor_2 = fork();
    if(floor_2<0) perror("create process 2 error!\n");
    else if(floor_2==0){	//child process 2
    	pthread_t control_panel,display_panel;
    	int error;

    	error = pthread_create(&control_panel,NULL/*thread attr*/,flr_control,NULL/*args*/);
		if(error!=0){
			perror("create user thread failed!\n");
			return -1;
		}
		else{
			printf("prthread create is created!\n");
		}

		error = pthread_create(&display_panel,NULL/*thread attr*/,flr_monitor,NULL/*args*/);
		if(error!=0){
			perror("create user thread failed!\n");
			return -1;
		}
		else{
			printf("prthread create is created!\n");
		}

		error = pthread_join(control_panel,NULL);
		if(error!=0){
			perror("pthread join failed!\n");
		}
		error = pthread_join(display_panel,NULL);
		if(error!=0){
			perror("pthread join failed!\n");
		}
    }

    floor_3 = fork();
    if(floor_3<0) perror("create process 3 error!\n");
    else if(floor_3==0){	//child process 3
    	pthread_t control_panel,display_panel;
    	int error;

    	error = pthread_create(&control_panel,NULL/*thread attr*/,flr_control,NULL/*args*/);
		if(error!=0){
			perror("create user thread failed!\n");
			return -1;
		}
		else{
			printf("prthread create is created!\n");
		}

		error = pthread_create(&display_panel,NULL/*thread attr*/,flr_monitor,NULL/*args*/);
		if(error!=0){
			perror("create user thread failed!\n");
			return -1;
		}
		else{
			printf("prthread create is created!\n");
		}

		error = pthread_join(control_panel,NULL);
		if(error!=0){
			perror("pthread join failed!\n");
		}
		error = pthread_join(display_panel,NULL);
		if(error!=0){
			perror("pthread join failed!\n");
		}
    }

    in_ele = fork();
    if(in_ele<0) perror("create process in_ele error!\n");
    else if(in_ele==0){		//child process in_ele
    	pthread_t control_panel,display_panel;
    	int error;

    	error = pthread_create(&monitor,NULL,ele_monitor,NULL);
		if(error!=0){
			perror("create user thread failed!\n");
			return -1;
		}
		else{
			printf("prthread create is created!\n");
		}

		error = pthread_create(&control,NULL,ele_control,NULL);
		if(error!=0){
			perror("create user thread failed!\n");
			return -1;
		}
		else{
			printf("prthread create is created!\n");
		}

		error = pthread_join(control,NULL);
		if(error!=0){
			perror("pthread join failed!\n");
		}
		error = pthread_join(monitor,NULL);
		if(error!=0){
			perror("pthread join failed!\n");
		}
    }

    // simulate elevator itself
    // �����������һ��¥��ʱ��Ϊ1s,�������Լ�����ʱ��ҲΪһ��
    init_elevator();
    elevator.floor = 1;	//�����߳̿�ʼ����
    printf("main loop begins!\n\n");
    while(1){

    	State temp =decision();
    	if(temp == stop) {
    		printf("elevator is stoped!\n"); // include open door, close door
    		sleep(1);
    		continue;
    	}
    	else if(temp == up){
    		printf("elevator is upward\n");
    		sleep(1);
    		elevator.floor++;
    		continue;
    	}
    	else if(temp == down){
    		printf("elevator is downward\n");
    		sleep(1);
    		elevator.floor--;
    		continue;
    	}
    	// else if(temp == open){	//open door
    	// 	printf("the door is open");
    	// 	sleep(0.5);
    	// 	continue;
    	// }
    	// else if(temp == close){ //close door
    	// 	printf("the door is close");
    	// 	sleep(0.5);
    	// 	continue;
    	// }
    }
    return 1;
}


void init_elevator()
{
    elevator.floor = 0;
    elevator.state = stop;
    elevator.gate = close;
}
void show_state()
{
    switch(elevator.state)
    {
	case up:printf("The elevator is going up.\n");
	case down:printf("The elevator is going down.\n");
	case stop:printf("The elevator is on floor %d.\n",elevator.floor);
    }
    switch(elevator.gate)
    {
	case open:printf("And the gate is open.\n");
	case close:printf("And the gate is close.\n");
    }
}
void ele_up(int des_flr)
{
    elevator.state = up;
    elevator.gate = close;

    while(elevator.floor < des_flr)
    {	
	elevator.floor++;
	sleep(5);
    }
    elevator.gate = open;
}
void ele_down(int des_flr)
{
   elevator.state = up;
   elevator.gate = close;
   while(elevator.floor > des_flr)
   {   
        elevator.floor--;
        sleep(5);
   }
  elevator.gate = open;
}

void *ele_monitor()//����������ʾ�� 
{
    printf("monitor in elevator \n");
    pthread_exit((void *)1);
    //show_state
}
void *ele_control()//�����п������ 
{
    printf("ele_control.\n");
    pthread_exit((void *)1);
    //opengate closegate des_flr
}

void *flr_monitor(void *args)//ÿ��¥����ʾ��� 
{
    printf("monitor in every floor\n");
    pthread_exit((void *)1);
    //show_state
    return NULL;
}

void *flr_control(void *args)//ÿ��¥������� 
{   
    printf("flr_control\n");
    pthread_exit((void *)1);
    //up down
    return NULL;
}

