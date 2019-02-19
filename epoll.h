//epoll.h

#ifndef EPOLL_H	
#define EPOLL_H	

#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include <unordered_map>

#include "pollevents.h"
#include "namespdef.h"


NAMESP_BEGIN
namespace net
{


template<class EventQueue>
class Epoll
{
	static constexpr int _max = 2048;
public:
	Epoll(EventQueue* q):_evt_queue(q){
		init();
	}

	bool attach(fd_t fd);
	void detach(fd_t fd){
		if( epoll_ctl(_efd, EPOLL_CTL_DEL, fd, NULL) == -1)
			perror("epoll_ctl");
	}

        void select();

	void postSend(fd_t fd){
		epoll_event tmp;
		tmp.data.fd = fd;
		tmp.events = EPOLLET|EPOLLOUT;
		if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ) 
			perror("epoll_ctl");
	}

	void postRecv(fd_t fd){
		epoll_event tmp;
		tmp.data.fd = fd;
		tmp.events = EPOLLET|EPOLLIN;
		if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ) 
			perror("epoll_ctl");
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

	template<class Event>
	void postEvent(fd_t fd);

private:
	int _efd;
	EventQueue* _evt_queue=nullptr;
};

}//namespace net
NAMESP_END

#include "epoll.inl"

 #endif /*EPOLL_H*/
