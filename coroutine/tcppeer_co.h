///udppeer_co.h

#ifndef	TCPPEER_CO_H
#define TCPPEER_CO_H

#include "tcppeer_co_basic.h"
#include "msg.h"
#include "event_queue_epoll.h"
#include "socket.h"

NAMESP_BEGIN
namespace net
{

using TcpPeer_co4 = TcpPeer_co<TcpSocket<IPv4>, EventQueueEpoll, MsgSt>;
using TcpPeer_co6 = TcpPeer_co<TcpSocket<IPv6>, EventQueueEpoll, MsgSt>;

}//net
NAMESP_END

#endif /*TCPPEER_CO_H*/
