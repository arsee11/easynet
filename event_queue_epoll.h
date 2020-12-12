///event_queue_epoll.h
//
//

#ifndef EVENT_QUEUE_EPOLL_H
#define EVENT_QUEUE_EPOLL_H

#include "epoll.h"
#include "event_queue.h"
#include "event.h"

NAMESP_BEGIN
namespace net
{

using EventQueueEpoll = EventQueue<Epoll<Event>>;

}//net
NAMESP_END

#endif /*EVENT_QUEUE_EPOLL_H*/
