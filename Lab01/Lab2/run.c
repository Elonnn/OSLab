#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <pthread.h>

int main(){
	//执行电梯程序
	system("gnome-terminal  -e ./floor1");  
	system("gnome-terminal  -e ./floor2");  
	system("gnome-terminal  -e ./floor3");  
	system("gnome-terminal  -e ./elevator_monitor");  
	system("gnome-terminal  -e ./elevator");
	return 0;
}
