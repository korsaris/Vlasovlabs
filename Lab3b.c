#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	time_t t;
	t = time(NULL);
  	struct tm* timeinfo;
	pid_t pid;
	int fd_fifo;
	char buff[80]={0};
	char* time;
	printf("Buffer before\"%s\"\n",buff);
	if((mkfifo("./fifo_file.fifo", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) == -1)
	{
		
		perror("can't make fifo\n");
		exit(1);
	}
	if((fd_fifo=open("./fifo_file.fifo",O_RDWR)) == -1)
	{	
		perror("can't open fifo\n");
		exit(1);
	}
	switch(pid=fork())
	{
		case -1: 
			printf("GAVNO"); 
			exit(1);
			break;
		case 0:
			//sleep(5);
			timeinfo = localtime(&t);
			strcpy(buff,asctime(timeinfo)); 
			write(fd_fifo,buff,sizeof(buff));
			exit(0);
			break;
		default:
			wait(NULL);
			read(fd_fifo,buff,sizeof(buff));
			break;
	}
	close(fd_fifo);
	printf("Buffer after %s",buff);
	return 0;
}
