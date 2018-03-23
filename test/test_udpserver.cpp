#include <iostream>
#include <string>
#include <sstream>
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
	
	if( (lfd=socket(AF_INET, SOCK_DGRAM, 0) )<0 )
	{
		perror("socket");
		return 1;
	}

	if( bind(lfd, (sockaddr*)&laddr, sizeof(laddr) )<0 )
	{
		perror("bind");
		return 1;
	}

	sockaddr_in raddr;
	memset(&raddr, 0, sizeof(raddr) );
	int rlen;
	socklen_t raddr_len=sizeof(raddr);;
	char buf[1024]={0};
	while(true)
	{
		if( (rlen=recvfrom(lfd, buf, 1024, 0, (sockaddr*)&raddr, &raddr_len) )<=0)
		{
			perror("recvfrom");
			return 1;
		}

		string rip = string( inet_ntoa(raddr.sin_addr) );
		unsigned short rport =  ntohs(raddr.sin_port);

		cout<<"remore ip:"<<rip<<endl;
		cout<<"remote port:"<<rport<<endl;
		
		stringstream ss;
		ss<<rport;
		string strport;
		ss>>strport;
		string msg = rip+":"+strport;
		sendto(lfd, msg.c_str(), msg.size(),0, (sockaddr*)&raddr, sizeof(raddr) );
	}
	
	getchar();
	close(lfd);
	return 0;
}
