//acceptor_test.cpp

#include "acceptor.h"
#include "event_queue_epoll.h"
#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;


void onAccept(fd_t fd)
{
	sockaddr_in addr;
	socklen_t len = sizeof(addr);	
	fd_t newfd = ::accept(fd, (sockaddr*)&addr, &len);
	cout<<"onAccept(fd="<<newfd<<"):"
		<<inet_ntoa(addr.sin_addr)<<" "<<ntohs(addr.sin_port)<<endl;
}

struct EQ{
};

struct EH{
};

int main()
{
	EventQueueEpoll eq;
	AcceptorR a(&eq, 10000);	
	AcceptorR a2(&eq, 10001);	

	a.listenOnAccept(&onAccept);
	a2.listenOnAccept(&onAccept);

	while(true)
	{
		eq.process();
	}
}
