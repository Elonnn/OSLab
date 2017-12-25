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
char work_area[WORK_SIZE];	//互斥访问
int cmds_buffer[MAX_COMMANDS];	// 12,13,21,23,31,32 注：12表示有用户希望从1楼前往2楼，其他类似
int cmds_processing[MAX_COMMANDS]; //正在处理的需求
int cmd_bptr = 0; //buffer ptr
int cmd_pptr = 0; //processing ptr
Elevator elevator;
int print_flags;
pthread_mutex_t work_mutex; //互斥锁


enum State{
    up,down,stop,open,close
};//电梯状态 上行\下行\停止,开门
enum gateState{
    open,close
};//电梯门 开\关 
struct Elevator{
    int floor;//当前楼层 
    State state;//运行状态 
    gateState gate;//门状态 
}; 

void init_elevator();//电梯初始化 
void show_state();//显示运行状态 
void ele_up(int des_flr);//上行 
void ele_down(int des_flr);//下行 

void *ele_monitor();//电梯中显示面板
void *ele_control();//电梯中控制面板 
void *flr_monitor();//每层楼的显示面板 
void *flr_control();//每层楼控制面板 


//电梯处于停止状态时，父进程先扫描等待需求和响应数组，然后运行decision函数，处理并发多需求
State decision(void){	//决定电梯控制逻辑，i.e. 上行，下行，停止
	// 电梯控制逻辑，根据cmds_buffer[MAX_COMMANDS]，cmds_processing[MAX_COMMANDS],以及电梯运行状态决定

}


int main(int argc,char *argv[])
{
    /*参考思路
	  父进程模拟电梯运行，负责改变楼层，创建四个子进程分别模拟1,2,3楼的控制面板及电梯内控制面板，负责I/O。

	  第一、 1,2,3楼的控制面板进程立即创建2个线程，等待用户，问题是怎么获取输入？
	  		在1,2,3,楼的控制线程内打开另一个linux终端，每个终端可以命名为floor X control panel，永远等待用户输入,可行性待确定

	  第二、 电梯内的控制面板进程中的线程可根据需要创建，即有人时创建，无人时消亡，也可一直存在

	  第三、建立一个足够长的互斥访问的数组存放用户输入(目的楼层1,2,3及用户所处楼层)，一个互斥访问的数组长度指针
	  		编写一个decision(int current_floor)函数模拟电梯控制逻辑，扫描上述数组决定电梯运行方式
	  		父进程循环检查上述数组，运行decision函数，
	*/ 
	
	pid_t floor_1,floor_2,floor_3,in_ele,elevator;//创建四个子进程 
    pid_t child;
    pthread_t monitor,control;//线程：显示电梯内部运行状态、控制面板 
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
    // 假设电梯上升一层楼的时间为1s,开关门以及启动时间也为一秒
    init_elevator();
    elevator.floor = 1;	//各个线程开始工作
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

void *ele_monitor()//电梯中显面示板 
{
    printf("monitor in elevator \n");
    pthread_exit((void *)1);
    //show_state
}
void *ele_control()//电梯中控制面板 
{
    printf("ele_control.\n");
    pthread_exit((void *)1);
    //opengate closegate des_flr
}

void *flr_monitor(void *args)//每层楼的显示面板 
{
    printf("monitor in every floor\n");
    pthread_exit((void *)1);
    //show_state
    return NULL;
}

void *flr_control(void *args)//每层楼控制面板 
{   
    printf("flr_control\n");
    pthread_exit((void *)1);
    //up down
    return NULL;
}

