#include"header.h"
#include<string.h>
bool verify()
{
	char * name;
	char * password;
	name = getName();
	password = getPassword();
	bool result =false;
	if((strcmp(name,"LiMing")==0)&&(strcmp(password,"LiMing")==0))
	{
		printf("LiMing ,you are in this system!\n");
		result = true;
	}
	else printf("Sorry,you are not LiMing,so you can not get in this system!\n");
	return result;
}
