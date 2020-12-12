///event_queue.h
//
//

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <memory>
#include <iostream>

NAMESP_BEGIN
namespace net
{

template<class Poller>
class EventQueue 
{ 
public:
	EventQueue()
	{
	}

	~EventQueue()
	{
	}

    void process();

	template<class Event>
	void bind(fd_t fd, Event* e);

	template<class Event>
	void unbind(fd_t fd, Event* e);

	void unbind(fd_t fd);

private:
	template<class Event>
	void process(Event* e){ e->fire(); }

private:
	Poller _poller;
};

}//net
NAMESP_END

#include "event_queue.inl"

#endif /*EVENT_QUEUE_H*/
