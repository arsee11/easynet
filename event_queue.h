///event_queue.h
//
//

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <memory>
#include <iostream>
#include "event.h"

NAMESP_BEGIN
namespace net
{

template<
	template<class>class Poller
>
class EventQueue 
{ 
public:
	EventQueue()
	{
	}

	~EventQueue()
	{
		std::cout<<"~NetEventQueueBasic()"<<std::endl;
	}

        void process();

	void bind(fd_t fd, event_ptr e);

	void unbind(fd_t fd);

private:
	void process(event_ptr e){ e->fire(); }

private:
	Poller<Event> _poller;
};

}//net
NAMESP_END

#include "event_queue.inl"

#endif /*EVENT_QUEUE_H*/
