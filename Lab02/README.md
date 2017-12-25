## Elevator-Scheduler

### 关于初始化的一些说明

``` c
/* in order to init readerCnt */
sem_t*cntInit;
cntInit=sem_open("cntInit",O_CREAT,0644,0);
if(cntInit==SEM_FAILED){
  perror("unable to create semaphore");
  sem_unlink("cntInit");
  exit(-1);
}
*readerCnt=0;
for(int i=0;i<NR_READER-1;i++)
  sem_post(cntInit);
```

每个读者进程尝试创建一个cntInit的信号变量, 但只有第一个读者进程, 拥有初始化的权限, 且初始化后应执行NR_READER - 1个sem_post, 以使得其他读者进程能继续执行.

初始化elevator status同理.