//acceptor.h

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "acceptor_reactor.h"
#include "acceptor_completed.h"
#include "event_queue_epoll.h"
#include "netpeer.h"
#include "msg.h"

#include <memory>

NAMESP_BEGIN
namespace net
{

///Reactor Acceptor
using AcceptorR = AcceptorReactor<EventQueueEpoll>;

using NetPeer = NetPeerBasic<EventQueueEpoll, MsgSt>;
using netpeer_ptr = std::shared_ptr<NetPeer>;

///Completed Acceptor
using AcceptorC =AcceptorCompleted<EventQueueEpoll, NetPeer, netpeer_ptr>; 

}//net
NAMESP_END

#endif /*ACCEPTOR_H*/
