//pollevents.h

#ifndef POLL_EVENTS_H
#define POLL_EVENTS_H

#include "netevents.h"

NAMESP_BEGIN
namespace net
{

template<class EventQueue>
class PollInputEvent : public NetInputEvent<EventQueue>  
{
public:
	PollInputEvent(const listener_list& ls, fd_t fd)
		:NetInputEvent<EventQueue>(ls, fd)
	{
	}
};

template<class EventQueue>
class PollOutputEvent : public NetInputEvent<EventQueue>  
{
public:
	PollOutputEvent(const listener_list& ls, fd_t fd)
		:NetInputEvent<EventQueue>(ls, fd)
	{
	}
};

template<class EventQueue>
class PollCloseEvent : public NetInputEvent<EventQueue> 
{
public:
	PollCloseEvent(const listener_list& ls, fd_t fd)
		:NetInputEvent<EventQueue>(ls, fd)
	{
	}
};

}//net
NAMESP_END

#endif /*POLL_EVENTS_H*/
