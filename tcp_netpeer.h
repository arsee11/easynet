
#ifndef TCP_NETPEER_H
#define TCP_NETPEER_H

#include "netpeer.h"
#include "msg.h"
#include "event_queue_epoll.h"
#include "socket.h"

NAMESP_BEGIN
namespace net
{

using NetPeer4 = NetPeerBasic<TcpSocket<IPv4>, EventQueueEpoll, MsgSt>;
using netpeer_ptr4 = std::shared_ptr<NetPeer4>;
using NetPeer6 = NetPeerBasic<TcpSocket<IPv6>, EventQueueEpoll, MsgSt>;
using netpeer_ptr6 = std::shared_ptr<NetPeer6>;

}//net
NAMESP_END

#endif /*TCP_NETPEER_H*/
