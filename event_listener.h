///event_listener.h
//
#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <list>
#include <map>
#include "fddef.h"
#include "event.h"
#include "namespdef.h"

NAMESP_BEGIN
namespace net
{


class EventListener
{
public:
	virtual ~EventListener(){}
		
};

using listener_list = std::list<EventListener*>;
using listener_map = std::map<fd_t, listener_list>;

}//net
NAMESP_END

#endif /*EVENT_LISTENER_H*/

