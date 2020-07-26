#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXBUF 1024

int main(int argc, char **argv)
{
    int sockfd, len;
    /* struct sockaddr_in dest; */ // IPv4
    struct sockaddr_in6 dest;      // IPv6
    char buf[MAXBUF + 1];
 
    if (argc != 3) {
        printf("usage: %s [ip6:port]\n", argv[0]);
        exit(0);
    }
    /* 创建一个 socket 用于 tcp 通信 */
    /* if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { */ // IPv4
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {      // IPv6
        perror("Socket");
        exit(errno);
    }
    printf("socket created/n");
 
    /* 初始化服务器端（对方）的地址和端口信息 */
    bzero(&dest, sizeof(dest));
    /* dest.sin_family = AF_INET; */  // IPv4
    dest.sin6_family = AF_INET6;     // IPv6
    /* dest.sin_port = htons(atoi(argv[2])); */ // IPv4
    dest.sin6_port = htons(atoi(argv[2]));     // IPv6
    /* if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) { */ // IPv4
    if ( inet_pton(AF_INET6, argv[1], &dest.sin6_addr) < 0 ) {                 // IPv6
        perror(argv[1]);
        exit(errno);
    }
    printf("address created\n");
 
    /* 连接服务器 */
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
        perror("Connect ");
        exit(errno);
    }
    printf("server connected\n");
 
    /* 接收对方发过来的消息，最多接收 MAXBUF 个字节 */
    bzero(buf, MAXBUF + 1);
    /* 接收服务器来的消息 */
    len = recv(sockfd, buf, MAXBUF, 0);
    if (len > 0)
        printf("recv[%d]:%s\n", len, buf);
    else
		perror("recv\n");
 
    bzero(buf, MAXBUF + 1);
    strcpy(buf, "world\n");
    /* 发消息给服务器 */
    len = send(sockfd, buf, strlen(buf), 0);
    if (len < 0)
		perror("send");
 
    /* 关闭连接 */
    close(sockfd);
    return 0;

}
