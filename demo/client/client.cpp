#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAXSIZE 1024
#define IPADDRESS "127.0.0.1"
#define SERV_PORT 8787
#define FDSIZE 1024
#define EPOLLEVENTS 20

static void handle_connection(int sockfd);
int main(int argc, char *argv[]) {
  int sockfd;
  char buf[MAXSIZE];
  struct sockaddr_in servaddr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
  if (int status =
          connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
      status < 0) {
    perror("connection error");
  }
  //处理连接
  int nwrite;
  nwrite = write(sockfd, buf, strlen(buf));
  if (nwrite == -1) {
    perror("write error:");
    close(sockfd);
  }
  close(sockfd);
  return 0;
}