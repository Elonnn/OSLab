#include"hello.h"
#include<string.h>
char* command(char * name)
{
		char *s;
		if (strcmp(name,"os")==0)
			s = "You are in os course!\n";
		else s = "You are not in os course!\n";
		return s;
}
void display(char *s)
{
	printf("%s",s);
}
char* getName()
{
	char *s;
	s = (char *)malloc(sizeof(char)*10);
	printf("Please enter your name:\n");
	scanf("%s",s);
	return s;
}
char* getPassword()
{
	char *s;
	s = (char *)malloc(sizeof(char)*10);
	printf("Please enter your password:\n");
	scanf("%s",s);
	return s;
}
