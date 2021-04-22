//acceptor_co.h

#ifndef ACCEPTOR_CO_H
#define ACCEPTOR_CO_H

#include "acceptor.h"
#include "tcppeer_co.h"

NAMESP_BEGIN
namespace net
{

template<class Acceptor, class EventQueueT, class TcpPeer>
class Acceptor_co
{ 
public:
    using peer_ptr = std::shared_ptr<TcpPeer>;

public:
	Acceptor_co(EventQueueT* q, const AddrPair& local_addr);

	Acceptor_co(EventQueueT* q, unsigned short port)
		:Acceptor_co(q, AddrPair{"", port})
	{
	}
	
    auto async_accept();

private:
    Acceptor _acceptor;
};

using Acceptor_co4 =Acceptor_co<AcceptorC4, EventQueueEpoll, TcpPeer_co4>; 
using Acceptor_co6 =Acceptor_co<AcceptorC6, EventQueueEpoll, TcpPeer_co6>;

}//net
NAMESP_END


#include "acceptor_co.inl"

#endif /*ACCEPTOR_CO_H*/
