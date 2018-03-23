#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char **argv)
{
	int lfd;
	sockaddr_in laddr;
	memset(&laddr, 0, sizeof(laddr));
	laddr.sin_family = AF_INET;
	laddr.sin_addr.s_addr = INADDR_ANY;
	laddr.sin_port = htons(11111);
	
	if( (lfd=socket(AF_INET, SOCK_STREAM, 0) )<0 )
	{
		perror("socket");
		return 1;
	}

	if( bind(lfd, (sockaddr*)&laddr, sizeof(laddr) )<0 )
	{
		perror("bind");
		return 1;
	}

	listen(lfd, 5);

	sockaddr_in raddr;
	memset(&raddr, 0, sizeof(raddr) );
	int rfd;
	socklen_t raddr_len=sizeof(raddr);;
	if( (rfd=accept(lfd, (sockaddr*)&raddr, &raddr_len) )<0)
	{
		perror("accept");
		return 1;
	}

	string rip = string( inet_ntoa(raddr.sin_addr) );
	unsigned short rport =  ntohs(raddr.sin_port);

	cout<<"remore ip:"<<inet_ntoa(raddr.sin_addr)<<endl;
	cout<<"remote port:"<<ntohs(raddr.sin_port)<<endl;
	
	send(rfd, rip.c_str(), rip.size(),0 );
	char *p = (char*)&rport;
	send(rfd, p, 2, 0);
	
	getchar();
	close(rfd);
	close(lfd);
	return 0;
}
