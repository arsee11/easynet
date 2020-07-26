///udppeer.h

#ifndef	UDPPEER_H 
#define UDPPEER_H 

#include "udppeer_basic.h"
#include "msg.h"
#include "event_queue_epoll.h"
#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

using UdpPeer = UdpPeerBasic<EventQueueEpoll, MsgSt>;

}//net
NAMESP_END

#endif /*UDPPEER_H*/
