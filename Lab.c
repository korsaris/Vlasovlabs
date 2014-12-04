#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
	time_t t = time(NULL);
  	struct tm* timeinfo  = localtime(&t);
	pid_t pid;
	int pfd[2];
	char buff[80];
	char* time;
	time=timeinfo;
	printf("%s",buff);
	if(pipe(pfd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	switch(pid=fork())
	{
		case -1: 
			printf("GAVNO"); 
			exit(1);
		case 0:
			strcpy(buff,time);
			close(pfd[0]); 
			write(pfd[1],buff,strlen(buff));
			close(pfd[1]);
			exit(0);
		default:
			close(pfd[1]);
			read(pfd[0],buff,strlen(buff));
			wait(NULL);
	}
	
	printf("%s\n",buff);
	return 0;
}
