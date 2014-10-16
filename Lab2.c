#include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	time_t t = time(NULL);
	struct tm* time = localtime(&t);
	pid_t pid;
	int n=0,size=pow(2,time->tm_hour%12);
	pid_t pid_array[size];
	while(n<size)
	{
		n++;
		switch(pid=fork())
		{
			case -1:
				printf("error!!");
				exit(1);
			case 0:
				printf("I'm a child №%d sin=%lf\n",n,sin(time->tm_min));
				exit(0);
				break;
			default:
				wait(NULL);
				pid_array[n]=pid;
		}
	}
	printf("Papka process sin=%lf\n",sin(time->tm_min));
	n=0;
	while(n<size)
	{
		waitpid(pid_array[n],NULL,0);
		n++;
	}
	return 0;
}
