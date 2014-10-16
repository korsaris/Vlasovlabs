#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{
	DIR *dir;
	struct dirent * entry;
	struct stat buff;
	char* fullpath[1024], buffer[256];

	if(argv[1]==0) 
	{
		dir = opendir("/");
		strcat(buffer,"/");
	}
	else 
	{
		dir = opendir(argv[1]);
		strcat(buffer,argv[1]);
	}
	if (!dir)
	{	
		printf("IncorrectDirectory\n");
		exit(1);
	}
	strcpy(fullpath,buffer);
	while ( (entry = readdir(dir)) != NULL)
	{	
		strcat(fullpath,"/");
		strcat(fullpath,entry->d_name);
		stat(fullpath,&buff);//выводы стырены у Андрея
		printf("filename: %s\n",entry->d_name);
		printf("filetype: %hhu\n",entry->d_type);
		printf("I-node number: %llu\n",buff.st_ino);
		printf("Mode: %ld (octal)\n",buff.st_mode);
		printf("Link count: %d\n",buff.st_nlink);
		printf("Ownership: UID=%d GID=%d\n",buff.st_uid,buff.st_gid);
		printf("Preferred I/O block size: %d bytes\n",buff.st_blksize);
		printf("File size: %lld bytes\n",buff.st_size);
		printf("Blocks allocated: %lld\n",buff.st_blocks);
		printf("Last status change: %s",ctime(&buff.st_ctime));
		printf("Last file access: %s",ctime(&buff.st_atime));
		printf("Last file modification: %s\n",ctime(&buff.st_mtime));
		strcpy(fullpath,buffer);
	}
	
	closedir(dir);
	return 0;
}
