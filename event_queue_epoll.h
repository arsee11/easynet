///event_queue_epoll.h
//
//

#ifndef EVENT_QUEUE_EPOLL_H
#define EVENT_QUEUE_EPOLL_H

#include "epoll.h"
#include "event_queue.h"

NAMESP_BEGIN
namespace net
{

using EventQueueEpoll = EventQueue<Epoll>;

}//net
NAMESP_END

#endif /*EVENT_QUEUE_EPOLL_H*/
