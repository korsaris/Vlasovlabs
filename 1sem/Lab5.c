#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{	
	time_t t;
	char * shm;
	char buff1[80]={0};
	int shmit;
	if((shmit=shmget(IPC_PRIVATE,sizeof(buff1),0666|IPC_CREAT|IPC_EXCL))==-1) perror("Shmger not created\n");
	switch(fork()){
		case 0:
			t = time(NULL);
  			struct tm* timeinfo;
			timeinfo = localtime(&t);
			shm = shmat(shmit,NULL,0);
			if (shm == (char *)(-1)) {
        		perror("child shmat");
        		exit(1);
    			}
			strcpy(buff1,asctime(timeinfo));
			strncpy(shm,buff1,sizeof(buff1));
			shmdt(shm);
			exit(0);
			break;
		case -1: 
			printf("Process not created\n");
			break;
		default:
			wait(NULL);
	}
	printf("Buffer before: %s\n",&buff1);
	shm = shmat(shmit,NULL,SHM_RDONLY);
	if (shm == (char *)(-1)) {
        perror("Parent shmat");
        exit(1);
    }
    strncpy(buff1,shm,sizeof(buff1));
	printf("Buffer after: %s",&buff1);
	shmdt(shm);
	return 0;
}
