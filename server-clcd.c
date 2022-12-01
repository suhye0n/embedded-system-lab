#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#define LCD_DEV_MAGIC 'Y'
#define LCD_INIT _IO(LCD_DEV_MAGIC, 0)
#define MSG_SIZE 1000
int main(void)
{
int sockfd;
struct sockaddr_in my_addr;
struct sockaddr_in their_addr;
int addr_len;
int myport = 5000;
int fd_clcd;
fd_clcd = open("/dev/clcd", O_RDWR);
char msg[MSG_SIZE];
int numbytes;
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
return 1;
my_addr.sin_family = AF_INET;
my_addr.sin_port = htons(myport);
my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
memset(&(my_addr.sin_zero), '\0', 8);
if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1)
return 1;
addr_len = sizeof(struct sockaddr);
while (1)
{
if ((numbytes = recvfrom(sockfd, msg, MSG_SIZE, 0, (struct sockaddr*)&their_addr, &addr_len)) == -1)
return 1;
msg[numbytes] = '\0';
printf("[RECV] %s (%d,%s)\n\n", msg, numbytes, inet_ntoa(their_addr.sin_addr));
numbytes = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&their_addr, sizeof(struct sockaddr));
if (numbytes == -1)
return 1;
printf("[SEND] %s (%d,%s)\n\n", msg, numbytes, inet_ntoa(their_addr.sin_addr));
int pos;
if (msg[0] == '~')
{
switch (msg[1]) {
case 'I':
ioctl(fd_clcd, LCD_INIT);
break;
default:
printf("unknown message\n");
break;
}
}
else
{
write(fd_clcd, msg, strlen(msg));
}
if (!strcmp(msg, "BYE") || !strcmp(msg, "bye"))
break;
}
close(sockfd);
close(fd_clcd);
return 0;
}
