///connector_co.h
//
//

#ifndef CONNECTOR_CO_H
#define CONNECTOR_CO_H

#include "connector.h"
#include "tcppeer_co.h"

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

//@param EventQueueT
//@param NetPeer
template<class Connector, class EventQueueT
	,class TcpPeer
>
class Connector_co
{
public:
    using peer_t = TcpPeer;

public:
	Connector_co(EventQueueT* q, uint16_t local_port)
        :Connector_co(q, AddrPair{"", local_port})
    {
	}

	Connector_co(EventQueueT* q, const AddrPair& local_addr)
        :_conn(q, local_addr)
	{
    }

	peer_t connect(const AddrPair& remote_addr){
        auto peer = _conn.connect(remote_addr);
        if(peer != nullptr ){
	        return TcpPeer(peer);
        }

        return TcpPeer(nullptr);
    }

protected:
    Connector _conn;
};


using Connector_co4 =Connector_co<Connector4, EventQueueEpoll, TcpPeer_co4>; 
using Connector_co6 =Connector_co<Connector6, EventQueueEpoll, TcpPeer_co6>;

}//net
NAMESP_END

#endif /*CONNECTOR_CO_H*/
