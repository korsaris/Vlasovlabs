
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
	time_t t;
	t = time(NULL);
  	struct tm* timeinfo;
	pid_t pid;
	int pfd[2];
	char buff[80]={0};
	char* time;
	printf("Buffer before\"%s\"\n",buff);
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
			break;
		case 0:
			//sleep(5);
			timeinfo = localtime(&t);
			strcpy(buff,asctime(timeinfo));
			close(pfd[0]); 
			write(pfd[1],buff,sizeof(buff));
			close(pfd[1]);
			exit(0);
			break;
		default:
			close(pfd[1]);
			read(pfd[0],buff,sizeof(buff));
			wait(NULL);
			break;
	}
	
	printf("Buffer after %s",buff);
	return 0;
}
