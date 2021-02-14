//acceptor.h

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "acceptor_reactor.h"
#include "acceptor_completed.h"
#include "event_queue_epoll.h"
#include "tcp_netpeer.h"
#include "msg.h"
#include "socket.h"

#include <memory>

NAMESP_BEGIN
namespace net
{

///Reactor Acceptor
using AcceptorR4 = AcceptorReactor<TcpSocket<IPv4>, EventQueueEpoll>;
using AcceptorR6 = AcceptorReactor<TcpSocket<IPv6>, EventQueueEpoll>;

///Completed Acceptor
using AcceptorC4 =AcceptorCompleted<TcpSocket<IPv4>, EventQueueEpoll, NetPeer4, netpeer_ptr6>; 
using AcceptorC6  =AcceptorCompleted<TcpSocket<IPv6>, EventQueueEpoll, NetPeer6, netpeer_ptr6>; 

}//net
NAMESP_END

#endif /*ACCEPTOR_H*/
