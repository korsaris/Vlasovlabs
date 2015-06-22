#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>


int main()
{
	int const size = 100;
	char buffer[size]={};
	int sock = socket(AF_INET,SOCK_STREAM,0);
	int connection_sock;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1488);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(sock == -1) 
		{
			printf("Socket error\n");
			return 1;
		}
	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) printf("Bind error\n");
	listen(sock,1);
	if((connection_sock = accept(sock, NULL, NULL)) == -1)
	{
		printf("Socket error\n");
		return 1;		
	}
	while(recvfrom(connection_sock,buffer,size,0,NULL,NULL)!=0)
	{
		printf("%s\n",buffer);
	}
	close(sock);
	return 0;
}