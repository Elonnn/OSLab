#include<stdio.h>
int func(int n)
{
	int sum =0,i;
	for(i =0;i<n;i++)
	{
		sum+=1;
	}
	return sum;
}
 main()
{
	int i;
	long result = 0;
	for(i=1;i<=100;i++)
	{
		result +=i;
	}
	printf("result[1-100] = %ld /n",result);
	printf("result[1-250] = %d /n",func(250));
}
