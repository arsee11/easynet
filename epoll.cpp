//epoll.cpp

#include "epoll.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

#include "acceptor.h"
#include "netpeer.h"

NAMESP_BEGIN
namespace net
{

inline static void setNoblocking(fd_t fd)
{
	int flag = fcntl(fd, F_GETFD, NULL);
	if( fcntl(fd, F_SETFL, flag|O_NONBLOCK) == -1 )
	{
		perror("fcntl");
		throw std::exception();
	}
}
	
void Epoll::attach(fd_t fd)
{
	setNoblocking(fd);
	epoll_event eh;
	eh.events = EPOLLET|EPOLLIN;//always in event.
	eh.data.fd = fd;
	if( epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &eh) == -1)
	{
		//error handle
		perror("epoll_ctl");
	}
}

Epoll::event_set_t Epoll::select()
{
#ifdef EBUG
	cout<<"EpollBase::selecting..."<<endl;
#endif

	epoll_event ehs[_max];
	int nfds = epoll_wait(_efd, ehs, _max, -1);
	if(nfds == -1)
	{
		perror("epoll_wait");
		return event_set_t();
	}

	event_set_t events;
	for(size_t i=0; i<nfds; ++i)
	{			
		if(ehs[i].events&EPOLLIN || ehs[i].events&EPOLLPRI)
		{
			Event* event;
			if(ehs[i].data.fd == _listenfd)
				event = new NetAcceptEvent(nullptr);
			else
				event = new NetInputEvent(new EventSender(ehs[i].data.fd) );	

			events.push_back(event);
		}
		else if(ehs[i].events&EPOLLOUT )
		{
			Event* event = new NetOutputEvent(new EventSender(ehs[i].data.fd) );
			events.push_back(event);
		}
		else if(ehs[i].events&EPOLLRDHUP )
		{
			Event* event = new NetCloseEvent(new EventSender(ehs[i].data.fd) );
			events.push_back(event);
		}
		else
		{
		}
	}

	return std::move(events);
}

}//net
NAMESP_END
