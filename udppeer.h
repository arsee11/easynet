///udppeer.h

#ifndef	UDPPEER_H 
#define UDPPEER_H 

#include "udppeer_basic.h"
#include "msg.h"
#include "netevent_queue.h"
#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

using UdpPeer = UdpPeerBasic<NetEventQueue, MsgSt>;

}//net
NAMESP_END

#endif /*UDPPEER_H*/
