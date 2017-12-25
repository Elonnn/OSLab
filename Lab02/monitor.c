#include <stdlib.h>
#include <stdio.h>

int main(){
	system("gnome-terminal  -e ./floor1");  
	system("gnome-terminal  -e ./floor2");  
	system("gnome-terminal  -e ./floor3");  
	system("gnome-terminal  -e ./panel");  
	system("./elevator");
	system("./closeSem");
	return 0;
}
