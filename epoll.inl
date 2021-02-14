//epoll.inl

//#include <iostream>
//using namespace std;

NAMESP_BEGIN
namespace net
{

inline static bool setNoblocking(fd_t fd)
{
	int flag = fcntl(fd, F_GETFD, NULL);
	if( fcntl(fd, F_SETFL, flag|O_NONBLOCK) == -1 )
	{
		perror("fcntl");
		return false;
	}
	return true;
}
	

template<class Event>
struct event_typeid;

template<>
struct event_typeid<NetInputEvent>
{
	void set(EventSource* esrc, Event* evt)
	{
		esrc->ievt = evt;
		esrc->events |=EPOLLIN;
	}

	void clear(EventSource* esrc)
	{
		esrc->ievt = nullptr;
		esrc->events &= ~EPOLLIN;
	}
};

template<>
struct event_typeid<NetOutputEvent>
{
	void set(EventSource* esrc, Event* evt)
	{
		esrc->oevt = evt;
		esrc->events |= EPOLLOUT;
	}

	void clear(EventSource* esrc)
	{
		esrc->oevt = nullptr;
		esrc->events &= ~EPOLLOUT;
	}
};

template<>
struct event_typeid<NetCloseEvent>
{
	void set(EventSource* esrc, Event* evt)
	{
		esrc->cevt = evt;
		esrc->events |=(EPOLLRDHUP|EPOLLHUP);
	}

	void clear(EventSource* esrc)
	{
		esrc->cevt = nullptr;
		esrc->events &= ~(EPOLLRDHUP|EPOLLHUP);
	}
};

template<class Event>
void set_events(EventSource* esrc, Event* evt)
{
	event_typeid<Event>().set(esrc, evt);		
}

template<class Event>
void clear_events(EventSource* esrc, Event* evt)
{
	event_typeid<Event>().clear(esrc);		
}

template<class EventT>
template<class Event>
bool Epoll<EventT>::addPollee(fd_t fd, Event* evt )
{
	//cout<<"addPollee fd:"<<fd<<endl;
	EventSource * esrc = nullptr;
	const auto& it = _evt_srcs.find(fd);
	if(it == _evt_srcs.end())
	{
		esrc = new EventSource;
		esrc->fd = fd;
		_evt_srcs[fd] = evt_src_ptr(esrc);
		set_events<Event>(esrc, evt);
		return add(fd, esrc);
	}

	esrc = it->second.get();

	set_events<Event>(esrc, evt);
	return modify(fd, esrc);
}

template<class EventT>
template<class Event>
bool Epoll<EventT>::delPollee(fd_t fd, Event* evt )
{
	EventSource * esrc = nullptr;
	const auto& it = _evt_srcs.find(fd);
	if(it == _evt_srcs.end())
		return false;

	esrc = it->second.get();
	clear_events<Event>(esrc, evt);
	return modify(fd, esrc);
}

template<class EventT>
bool Epoll<EventT>::delPollee(fd_t fd)
{
	//cout<<"delPollee fd:"<<fd<<endl;
	_evt_srcs.erase(fd);
	if( epoll_ctl(_efd, EPOLL_CTL_DEL, fd, NULL) == -1)
	{
		perror("epoll_ctl");
		return false;
	}

	return true;
}

template<class EventT>
bool Epoll<EventT>::add(fd_t fd, EventSource* src)
{
	if( !setNoblocking(fd))
		return false;

	//cout<<"add event fd:"<<fd<<" events:"<<src->events<<endl;
	epoll_event eh;
	eh.events = EPOLLET|src->events;
	eh.data.ptr = src;
	if( epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &eh) == -1){
		//error handle
		perror("epoll_ctl");
		return false;
	}

	return true;
}

template<class EventT>
bool Epoll<EventT>::modify(fd_t fd, EventSource* src)
{
	//cout<<"modify event fd:"<<fd<<" events:"<<src->events<<endl;
	epoll_event tmp;
	tmp.data.ptr = src;
	tmp.events = EPOLLET|src->events;
	if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ){
		perror("epoll_ctl");
		return false;
	}

	return true;
}

template<class EventT>
typename Epoll<EventT>::event_list Epoll<EventT>::select()
{
	event_list events;
	epoll_event ehs[_max];
    int nfds = epoll_wait(_efd, ehs, _max, -1);
	if(nfds == -1){
		perror("epoll_wait");
		return events; 
	}

    for(int i=0; i<nfds; ++i){			
		EventSource* esrc = static_cast<EventSource*>(ehs[i].data.ptr);
		//cout<<"select results fd:"<<esrc->fd<<" events:"<<ehs[i].events<<endl;
		if(ehs[i].events & EPOLLIN || ehs[i].events & EPOLLPRI){
			events.push_back(esrc->ievt);
			//cout<<"select results push input\n";
		}
		if(ehs[i].events & EPOLLOUT ){
			events.push_back(esrc->oevt);
			//cout<<"select results push output\n";
		}
		if(ehs[i].events & EPOLLRDHUP ){
			events.push_back(esrc->cevt);
			//cout<<"select results push close\n";
		}
	}

	return events;
}

}//net
NAMESP_END
