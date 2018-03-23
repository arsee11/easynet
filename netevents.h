//netevents.h

#ifndef NET_EVENTS_H
#define NET_EVENTS_H

#include "event.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class NetInputEvent : public Event
{
public:
	NetInputEvent(EventSender* s)
		:Event(s)
	{
	}
};

class NetOutputEvent : public Event
{
public:
	NetOutputEvent(EventSender* s)
		:Event(s)
	{
	}
};

class NetCloseEvent: public Event
{
public:
	NetCloseEvent(EventSender* s)
		:Event(s)
	{
	}
};

class NetAcceptEvent : public Event
{
public:
	NetAcceptEvent(EventSender* s)
		:Event(s)
	{
	}
};

}//net
NAMESP_END

#endif /*NET_EVENTS_H*/
