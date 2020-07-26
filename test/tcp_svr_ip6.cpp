
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAXBUF 1024
int main(int argc, char **argv)
{
    int sockfd, new_fd;
    socklen_t len;
 
    /* struct sockaddr_in my_addr, their_addr; */ // IPv4
    struct sockaddr_in6 my_addr, their_addr; // IPv6
 
    unsigned int myport, lisnum;
    char buf[MAXBUF + 1];
 
    if (argv[1])
        myport = atoi(argv[1]);
    else
        myport = 7838;
 
    if (argv[2])
        lisnum = atoi(argv[2]);
    else
        lisnum = 2;
 
    /* if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) { */ // IPv4
    if ((sockfd = socket(PF_INET6, SOCK_STREAM, 0)) == -1) { // IPv6
        perror("socket");
        exit(1);
    } else
        printf("socket created\n");
 
    bzero(&my_addr, sizeof(my_addr));
    /* my_addr.sin_family = PF_INET; */ // IPv4
    my_addr.sin6_family = PF_INET6;    // IPv6
    /* my_addr.sin_port = htons(myport); */ // IPv4
    my_addr.sin6_port = htons(myport);   // IPv6
    if (argv[3])
        /* my_addr.sin_addr.s_addr = inet_addr(argv[3]); */ // IPv4
        inet_pton(AF_INET6, argv[3], &my_addr.sin6_addr);  // IPv6
    else
        /* my_addr.sin_addr.s_addr = INADDR_ANY; */ // IPv4
        my_addr.sin6_addr = in6addr_any;            // IPv6
 
    /* if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) */ // IPv4
    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in6))  // IPv6
        == -1) {
        perror("bind");
        exit(1);
    } else
        printf("binded\n");
 
    if (listen(sockfd, lisnum) == -1) {
        perror("listen");
        exit(1);
    } else
        printf("begin listen\n");
 
    while (1) {
        len = sizeof(struct sockaddr);
        if ((new_fd =
             accept(sockfd, (struct sockaddr *) &their_addr,
                    &len)) == -1) {
            perror("accept");
            exit(errno);
        } else
            printf("server: connection[%s]:%d\n",
                   inet_ntop(AF_INET6, &their_addr.sin6_addr, buf, sizeof(buf)), // IPv6
                   ntohs(their_addr.sin6_port)); // IPv6
 
        /* 开始处理每个新连接上的数据收发 */
        bzero(buf, MAXBUF + 1);
        strcpy(buf,"hello");

        /* 发消息给客户端 */
        len = send(new_fd, buf, strlen(buf), 0);
        if (len < 0) {
			perror("send");
        } 
 
        bzero(buf, MAXBUF + 1);
        /* 接收客户端的消息 */
        len = recv(new_fd, buf, MAXBUF, 0);
        if (len > 0)
            printf("recv[%d]:%s\n",len, buf);
        else
			perror("recv");

        /* 处理每个新连接上的数据收发结束 */
    }
 
    close(sockfd);
    return 0;
}
