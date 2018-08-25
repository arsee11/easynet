///event.h

#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include "namespdef.h"

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

using event_ptr = std::shared_ptr<Event>;

}//net
NAMESP_END

#endif /*EVENT_H*/
