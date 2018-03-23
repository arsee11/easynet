//epoll.h

#ifndef EPOLL_H	
#define EPOLL_H	

#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include <vector>
#include <stdio.h>

#ifndef FD_DEF_H 
#include "fddef.h"
#endif

#include "netevents.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


NAMESP_BEGIN
namespace net
{
class Epoll
{
public:
	typedef std::vector<Event*> event_set_t;

public:
	Epoll(){
		init();
	}

	void listenfd(fd_t fd)
	{
		_listenfd = fd;
		attach(fd);
	}

	void attach(fd_t fd);
	void detach(fd_t fd){
		if( epoll_ctl(_efd, EPOLL_CTL_DEL, fd, NULL) == -1)
		{
			perror("epoll_ctl");
		}
	}

	event_set_t select();

	void postSend(fd_t fd){
		epoll_event tmp;
		tmp.data.fd = fd;
		tmp.events = EPOLLET|EPOLLOUT;
		if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ) 
		{
			perror("epoll_ctl");
		}
	}

	void postRecv(fd_t fd){
		epoll_event tmp;
		tmp.data.fd = fd;
		tmp.events = EPOLLET|EPOLLIN;
		if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ) 
		{
			perror("epoll_ctl");
		}
	}

private:
	void init(){
		_efd = epoll_create(_max);
		if( _efd == -1 )
		{
			perror("epoll_create");
			throw std::exception();
		}
	}

private:
	fd_t _efd;
	fd_t _listenfd;
	static constexpr int _max = 2048;
};

}//namespace net
NAMESP_END

 #endif /*EPOLL_H*/
