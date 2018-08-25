//epoll.inl


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
	
template<class EventQueue>
bool Epoll<EventQueue>::attach(fd_t fd)
{
	if( !setNoblocking(fd))
		return false;

	epoll_event eh;
	eh.events = EPOLLET|EPOLLIN;//always in event.
	eh.data.fd = fd;
	if( epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &eh) == -1)
	{
		//error handle
		perror("epoll_ctl");
		return false;
	}

	return true;
}

template<class EventQueue>
template<class Event>
void Epoll<EventQueue>::postEvent(fd_t fd)
{
	const listener_map& lmap= _evt_queue->findListenerMap(
		std::type_index(typeid(Event))
	);

	if(lmap.size() > 0 )
	{
		auto l=lmap.find(fd);
		if( l!= lmap.end())
			_evt_queue->pushEvent( event_ptr(new Event(l->second, fd)) );
	}	
}

template<class EventQueue>
void Epoll<EventQueue>::select()
{
	epoll_event ehs[_max];
	int nfds = epoll_wait(_efd, ehs, _max, -1);
	if(nfds == -1)
	{
		perror("epoll_wait");
		return; 
	}

	if(_evt_queue == nullptr)
	{
		//event queue null
		return;
	}

	for(size_t i=0; i<nfds; ++i)
	{			
		fd_t fd = ehs[i].data.fd;
		if(ehs[i].events&EPOLLIN || ehs[i].events&EPOLLPRI)
		{
			postEvent<PollInputEvent<EventQueue>>(fd);
		}
		else if(ehs[i].events&EPOLLOUT )
		{
			postEvent<PollOutputEvent<EventQueue>>(fd);
		}
		else if(ehs[i].events&EPOLLRDHUP )
		{
			postEvent<PollCloseEvent<EventQueue>>(fd);
		}
		else
		{
		}
	}

}

}//net
NAMESP_END
