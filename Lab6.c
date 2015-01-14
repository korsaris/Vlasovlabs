#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * thread_write(void *arg)
{
	pthread_mutex_lock(&mutex);
	time_t t;
	t = time(NULL);
  	struct tm* timeinfo;
	timeinfo = localtime(&t);
	char buff[80]={0};
	strcpy(buff,asctime(timeinfo));
	strncpy(arg,buff,sizeof(buff));
	sleep(3);
	pthread_mutex_unlock(&mutex);
	fflush(stdout);
	return 0;
}


int main()
{
	char buff1[80]={0};
	pthread_t tid;
	int shmit,thread_create_status;
	char * shm;
	if((shmit=shmget(IPC_PRIVATE,sizeof(buff1),0666|IPC_CREAT|IPC_EXCL)) == -1) perror("Shmger not created\n");
	if((shm = shmat(shmit,NULL,0)) == (char *)(-1)) perror("child shmat");
	thread_create_status = pthread_create(&tid,NULL,thread_write,(void*)shm);
	if(thread_create_status == 0){
		printf("thread create %d\n", tid);
	}else{
		perror("Thread not created:");
		exit(-10);
	}
	while(pthread_mutex_trylock(&mutex) == EBUSY)
	{
		printf("Main thread waiting\n");
		sleep(1);
	}
	printf("Buffer before: %s\n",&buff1);
	strncpy(buff1,shm,sizeof(buff1));
	pthread_mutex_unlock(&mutex);
	printf("Buffer after: %s",&buff1);
	shmdt(shm);
	pthread_mutex_destroy(&mutex);
	return 0;
}
