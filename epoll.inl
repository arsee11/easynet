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
	
template<class EventT>
bool Epoll<EventT>::addInput(fd_t fd, EventT* evt)
{
	if( !setNoblocking(fd))
		return false;

	epoll_event eh;
	eh.events = EPOLLET|EPOLLIN;//always in event.
	eh.data.ptr = evt;
	if( epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &eh) == -1)
	{
		//error handle
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
        int nfds = epoll_wait(_efd, ehs, _max, 1000);
	if(nfds == -1)
	{
		perror("epoll_wait");
		return events; 
	}

        for(int i=0; i<nfds; ++i)
	{			
		EventT* evt = static_cast<EventT*>(ehs[i].data.ptr);
		events.push_back(evt);
		//EventSource esrc = ehs[i].data.ptr;
		/*if(ehs[i].events&EPOLLIN || ehs[i].events&EPOLLPRI)
		{
			_event_queue->pushEvent(esrc->input_event());
		}
		else if(ehs[i].events&EPOLLOUT )
		{
			_event_queue->pushEvent(esrc->output_event());
		}
		else if(ehs[i].events&EPOLLRDHUP )
		{
			_event_queue->pushEvent(esrc->close_event());
		}
		else
		{
		}
		*/
	}

	return events;

}

}//net
NAMESP_END
