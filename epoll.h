//epoll.h

#ifndef EPOLL_H	
#define EPOLL_H	

#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include <unordered_map>
#include "event.h"
#include <vector>

#include "namespdef.h"


NAMESP_BEGIN
namespace net
{


template<class EventT>
class Epoll
{
	static constexpr int _max = 2048;

	struct EventTSource{
		EventT* ievt=nullptr;
		EventT* oevt=nullptr;
		EventT* cevt=nullptr;	
	};

	using event_list = std::vector<EventT*>;
public:
	Epoll(){
		init();
	}

	bool addInput(fd_t fd, EventT* evt );
	bool addOutput(fd_t fd, EventT* evt );
	bool addClose(fd_t fd, EventT* evt );

	void detach(fd_t fd){
		if( epoll_ctl(_efd, EPOLL_CTL_DEL, fd, NULL) == -1)
			perror("epoll_ctl");
	}

        event_list select();

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
private:
	int _efd;
};

}//namespace net
NAMESP_END

#include "epoll.inl"

 #endif /*EPOLL_H*/
