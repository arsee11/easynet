///event_listener.h
//
#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class Event;

class EventListener
{
public:
	virtual void onEvent(Event* e)=0;
		
};

}//net
NAMESP_END

#endif /*EVENT_LISTENER_H*/
