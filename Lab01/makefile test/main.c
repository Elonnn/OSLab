#include<stdio.h>
#include"hello.h"
#include"header.h"
int main()
{
	if(!verify())return 0;
	call();
	char * (*fun)(char *);
	fun = &command;
	char * s;
	s = (char *)malloc(10*sizeof(char));
	scanf("%s",s);
	display((*fun)(s));
	return 0;
}

