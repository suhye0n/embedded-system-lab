#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#define MSG_SIZE 1000
int main()
{
int sockfd;
struct sockaddr_in their_addr;
struct hostent* he;
char ipaddr[] = "192.168.0.9";
int port = 5000;
int numbytes;
char msg[MSG_SIZE];
if ((he = gethostbyname(ipaddr)) == NULL)
return 1;
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
return 1;
their_addr.sin_family = AF_INET;
their_addr.sin_port = htons(port);
their_addr.sin_addr = *((struct in_addr*)he->h_addr);
//memset(&(their_addr.sin_zero), 0, 8);


while(1)
{
	printf("input : ");

	

	//scanf("%s", msg);
	fgets(msg, MSG_SIZE, stdin);
	if(msg[strlen(msg)-1] == '\n')
	msg[strlen(msg)-1] = '\0';
	numbytes = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&their_addr, sizeof(struct sockaddr));
	if (numbytes == -1)
	return 1;

	printf("[SEND] %s (%d,%s)\n", msg, numbytes, inet_ntoa(their_addr.sin_addr));

	recvfrom(sockfd, msg, MSG_SIZE, 0, (struct sockaddr*)&their_addr, &addr_len);

	if (!strcmp(msg, "BYE") || !strcmp(msg, "bye"))
		break;
}

close(sockfd);
return 0;
}
