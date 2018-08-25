//acceptor.h

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "acceptor_basic.h"
#include "acceptor_reactor.h"
#include "acceptor_completed.h"
#include "netevent_queue.h"
#include "netpeer.h"
#include "msg.h"

#include <memory>

NAMESP_BEGIN
namespace net
{

using Acceptor=AcceptorBasic<NetEventQueue>; 


///Reactor Acceptor
using AcceptorR = AcceptorReactor<NetEventQueue, Acceptor>;

using NetPeer = NetPeerBasic<NetEventQueue, MsgSt>;
using netpeer_ptr = std::shared_ptr<NetPeer>;

///Completed Acceptor
using AcceptorC =AcceptorCompleted<NetEventQueue, Acceptor, NetPeer, netpeer_ptr>; 

}//net
NAMESP_END

#endif /*ACCEPTOR_H*/
