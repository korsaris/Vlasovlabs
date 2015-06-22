#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>


int main()
{
	int const size = 100;
	char buffer[size]={"Hello bro!"};
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1488);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if(sock == -1) 
		{
			printf("Socket error\n");
			return 1;
		}
	int i = sendto(sock, buffer, size, 0, &addr, sizeof(addr));
	if( i <= 0) printf("Send error\n");
	printf("%d\n",i);
	close(sock);
	return 0;
}