///netevent_queue.h
//
//

#ifndef NETEVENT_QUEUE_H
#define NETEVENT_QUEUE_H

#include "event_queue.h"
#include "netevent_queue_basic.h"

#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

using NetEventQueue = NetEventQueueBasic<EventQueue>;

}//net
NAMESP_END

#endif /*NETEVENT_QUEUE_H*/
