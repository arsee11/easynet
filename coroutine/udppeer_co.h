///udpppeer_co.h

#ifndef	UDPPEER_CO_H
#define UDPPEER_CO_H

#include "udppeer_co_basic.h"
#include "msg.h"
#include "event_queue_epoll.h"
#include "socket.h"

NAMESP_BEGIN
namespace net
{

using UdpPeer_co4 = UdpPeer_co<UdpSocket<IPv4>, EventQueueEpoll, MsgSt>;
using UdpPeer_co6 = UdpPeer_co<UdpSocket<IPv6>, EventQueueEpoll, MsgSt>;

}//net
NAMESP_END

#endif /*UDPPEER_CO_H*/
