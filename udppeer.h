///udppeer.h

#ifndef	UDPPEER_H 
#define UDPPEER_H 

#include "udppeer_basic.h"
#include "msg.h"
#include "event_queue_epoll.h"
#include "socket.h"
#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

using UdpPeer4 = UdpPeerBasic<UdpSocket<IPv4>, EventQueueEpoll, MsgSt>;
using UdpPeer6 = UdpPeerBasic<UdpSocket<IPv6>, EventQueueEpoll, MsgSt>;

}//net
NAMESP_END

#endif /*UDPPEER_H*/
