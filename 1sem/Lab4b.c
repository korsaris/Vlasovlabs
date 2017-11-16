#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>


void * thread_write(void *arg)
{
	time_t t;
	t = time(NULL);
  	struct tm* timeinfo;
	timeinfo = localtime(&t);
	char buff[80]={0};
	strcpy(buff,asctime(timeinfo));
	write((int)arg,buff,sizeof(buff));
	fflush(stdout);
	return 0;
}


int main()
{
	char buff1[80]={0};
	pthread_t tid;
	int fd_fifo;
	printf("Buffer before \"%s\"\n",&buff1);
	if((mkfifo("./fifo_file.fifo", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
	{	
		perror("can't make fifo\n");
		exit(1);
	}
	if((fd_fifo = open("./fifo_file.fifo",O_RDWR)) == -1)
	{	
		perror("can't open fifo\n");
		exit(1);
	}
	if (pthread_create(&tid,NULL,thread_write,fd_fifo)!=0)
	{
		perror("Thread not created:");
		exit(-10);
	}
	//pthread_join(tid, NULL);
	read(fd_fifo,buff1,sizeof(buff1));
	system("rm fifo_file.fifo");
	printf("%s",&buff1);
	return 0;
}