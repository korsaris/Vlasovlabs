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
#include <ctype.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void * thread_write(void *arg)
{
	time_t t;
	t = time(NULL);
  	struct tm* timeinfo;
	timeinfo = localtime(&t);
	char buff[80]={0};
	strcpy(buff,asctime(timeinfo));
	pthread_rwlock_rdlock(&rwlock);
	printf("Another thread write\n");
	strncpy(arg,buff,sizeof(buff));
	pthread_rwlock_unlock(&rwlock);
	sleep(3);
	fflush(stdout);
	return 0;
}


int main(int argc, char* argv[])
{
	char buff1[80]={0};
	pthread_t tid;
	int shmit,thread_create_status;
	char * shm;
	printf("%d\n",'1');
	if((shmit=shmget(IPC_PRIVATE,sizeof(buff1),0666|IPC_CREAT|IPC_EXCL)) == -1) perror("Shmger not created\n");
	if((shm = shmat(shmit,NULL,0)) == (char *)(-1)) perror("child shmat");
	thread_create_status = pthread_create(&tid,NULL,thread_write,(void*)shm);
	if(thread_create_status == 0){
		printf("thread create %d\n", tid);
	}else{
		perror("Thread not created:");
		exit(-10);
	}
	printf("Buffer before: %s\n",&buff1);
	if(argc >1){
		sleep(isalnum(argv[1][0])*argv[1][0]%'0');	
	}
	pthread_rwlock_wrlock(&rwlock);
	printf("Main thread read\n");
	strncpy(buff1,shm,sizeof(buff1));
	pthread_rwlock_unlock(&rwlock);
	printf("Buffer after: %s\n",&buff1);
	pthread_rwlock_destroy(&rwlock);
	shmdt(shm);
	return 0;
}
