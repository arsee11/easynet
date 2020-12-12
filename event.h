///event.h

#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include "namespdef.h"
#include "event_listener.h"
#include <list>
#include <algorithm>

NAMESP_BEGIN
namespace net
{


//////////////////////////////////////////////////////////////////////////
class Event
{
public:
	virtual ~Event()
	{
	}

	virtual void fire()=0;

};

using event_ptr = Event*;

template<class EventHandler>
class EventBasic: public Event
{
public:
        EventBasic(EventHandler h)
                :_handler(h)
        {}

        void setHandler(EventHandler h){ _handler = h; }

        void fire()override{
            if(_handler != nullptr){
                _handler();
            }
        };

protected:
        EventHandler _handler;
};

}//net
NAMESP_END

#endif /*EVENT_H*/
