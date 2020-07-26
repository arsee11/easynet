//acceptor_test.cpp

#include "../acceptor.h"


#include "../netevents.h"
#include "../event_queue_epoll.h"
#include "../event_listener_basic.h"
#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;


void onAccept(fd_t fd)
{
	sockaddr_in addr;
	socklen_t len = sizeof(addr);	
	fd_t newfd = ::accept(fd, (sockaddr*)&addr, &len);
	cout<<"onAccept("<<newfd<<"):"
		<<inet_ntoa(addr.sin_addr)<<" "<<ntohs(addr.sin_port)<<endl;
}

struct EQ{
};

struct EH{
};

template<class EventQueueT>
using AcceptListener = typename NetAcceptEvent<EventQueueT>::listener_t;

template<class EventQueueT>
using NNListener = EventListenerBasic<EventQueueT, EH>;  

int main()
{

	//EventQueueEpoll eq;
	//EQ eeq;
	//AcceptListener<EQ> l(&eeq);
	//NListener<EventQueueEpoll> l(&eq);
	//NetAcceptEvent<EventQueueEpoll>::listener_t* ll=new NetAcceptEvent<EventQueueEpoll>::listener_t(&eq);
	EventQueueEpoll eq;
	AcceptorR a(&eq, 10000);	
	AcceptorR a2(&eq, 10001);	

	a.listenOnAccept(&onAccept);

	while(true)
	{
		eq.process();
	}
}
