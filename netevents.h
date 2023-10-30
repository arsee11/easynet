//netevents.h

#ifndef NET_EVENTS_H
#define NET_EVENTS_H

#include "event.h"
#include "fddef.h"
#include "addr.h"
#include "namespdef.h"

#include <tuple>
#include <functional>
#include <assert.h>

NAMESP_BEGIN
namespace net
{


using EventHandler=std::function<void()>;

class NetInputEvent : public EventBasic<EventHandler>
{
public:
	NetInputEvent(EventHandler h)
		:EventBasic<EventHandler>(h)
	{
	}

};


class NetCloseEvent : public NetInputEvent
{
public:
	NetCloseEvent(EventHandler h)
		:NetInputEvent(h)
	{
	}
};

class NetOutputEvent: public NetInputEvent
{
public:
	NetOutputEvent(EventHandler h)
		:NetInputEvent(h)
	{
	}

};

////////////////////////////////////////////////////////////////////////////////
}//net
NAMESP_END

#endif /*NET_EVENTS_H*/
