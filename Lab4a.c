#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int pfd[2];

void * thread_write(void *arg)
{
	time_t t;
	t = time(NULL);
  	struct tm* timeinfo;
	timeinfo = localtime(&t);
	char buff[80]={0};
	strcpy(buff,asctime(timeinfo));
	// close(pfd[0]); 
	write(pfd[1],buff,sizeof(buff));
	// close(pfd[1]);
	printf("thread word end %d\n", pthread_self());
	fflush(stdout);
	return 0;
}


int main()
{
	char buff1[80]={0};
	pthread_t tid;
	int ** res = (int**)malloc(sizeof(int));
	printf("Start lab4\n");
	if(pipe(pfd) == -1)
	{	
		printf("Sux pipe not create");
		perror("pipe");
		exit(1);
	}
	
	printf("Create pthread\n");
	int pthread_res = pthread_create(&tid,NULL,thread_write,NULL);
	if (pthread_res == 0 ) {
		printf("thread create %d\n", tid);
	}else{
		perror("Thread not created:");
		exit(-10);
	}
	
	pthread_join(tid, NULL);
	printf("main thread %d has been waited\n", pthread_self());
	// sleep(10);
	// close(pfd[1]);
	read(pfd[0],buff1,sizeof(buff1));
	printf("hello");
	printf("%s \n",&buff1);
	return 0;
}
