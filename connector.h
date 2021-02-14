///connector.h
//
//

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "addr.h"
#include "tcp_netpeer.h"

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

//@param EventQueueT
//@param NetPeer
template<class Socket, class EventQueueT
	,class NetPeer, class NetPeerPtr
>
class Connector
{
public:
        using netpeer_ptr = NetPeerPtr;

public:
	Connector(EventQueueT* q, uint16_t local_port)
        :Connector(q, AddrPair{"", local_port})
    {
	}

	Connector(EventQueueT* q, const AddrPair& local_addr)
        :_evt_queue(q)
        ,_local_addr(local_addr)
	{
    }

	NetPeerPtr connect(const AddrPair& remote_addr){
        Socket socket(_local_addr);
        if( socket.connect(remote_addr) ){
	        return NetPeerPtr( new NetPeer(_evt_queue, socket) );
        }

        return nullptr;
    }

protected:
    EventQueueT* _evt_queue=nullptr;
    AddrPair _local_addr;
};


using Connector4 =Connector<TcpSocket<IPv4>, EventQueueEpoll, NetPeer4, netpeer_ptr4>; 
using Connector6 =Connector<TcpSocket<IPv6>, EventQueueEpoll, NetPeer6, netpeer_ptr6>; 

}//net
NAMESP_END

#endif /*CONNECTOR_H*/
