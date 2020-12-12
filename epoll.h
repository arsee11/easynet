//epoll.h

#ifndef EPOLL_H	
#define EPOLL_H	

#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include "event.h"
#include <vector>
#include <map>
#include <memory>

#include "namespdef.h"


NAMESP_BEGIN
namespace net
{

struct EventSource{
	Event* ievt=nullptr;
	Event* oevt=nullptr;
	Event* cevt=nullptr;	
	int fd=-1;
	int events=0;
};


template<class EventT>
class Epoll
{
	static constexpr int _max = 2048;
	using event_list = std::vector<EventT*>;
	using evt_src_ptr=std::unique_ptr<EventSource>;

public:
	Epoll(){
		init();
	}

	template<class Event>
	bool addPollee(fd_t fd, Event* evt );

	//delete @evt event attach to the fd
	template<class Event>
	bool delPollee(fd_t fd, Event* evt);

	//delete all events attach to the fd
	bool delPollee(fd_t fd);

	//@return available events
    event_list select();

private:
	bool add(fd_t fd, EventSource* evt );
	bool modify(fd_t fd, EventSource* src);
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
	std::map<int, evt_src_ptr> _evt_srcs;
};

}//namespace net
NAMESP_END

#include "epoll.inl"

#endif /*EPOLL_H*/
