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

int endofwrite = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void * thread_write(void *arg)
{
	sleep(2);
	pthread_mutex_lock(&mutex);
	printf("lock by another thread\n");
	time_t t;
	t = time(NULL);
  	struct tm* timeinfo;
	timeinfo = localtime(&t);
	char buff[80]={0};
	strcpy(buff,asctime(timeinfo));
	sleep(3);
	if(strncpy(arg,buff,sizeof(buff))!=0)
	{
		endofwrite = 0;
		pthread_cond_signal(&condition);
	}
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
	pthread_mutex_lock(&mutex);
	printf("lock by main thread\n");
	if(endofwrite == 0)
	{
		printf("Main thread wait...\n");
		pthread_cond_wait(&condition, &mutex);
	}
	printf("Buffer before: %s\n",&buff1);
	strncpy(buff1,shm,sizeof(buff1));
	pthread_mutex_unlock(&mutex);
	printf("Buffer after: %s",&buff1);
	shmdt(shm);
	pthread_cond_destroy(&condition);
	pthread_mutex_destroy(&mutex);
	return 0;
}
