///udppeer_co_basic.h

#ifndef	UDPPEER_CO_BASIC_H
#define UDPPEER_CO_BASIC_H

#include "fddef.h"
#include "addr.h"
#include "udppeer.h"
#include "netpeer_co.h"
#include "namespdef.h"
#include <coroutine>
#include <queue>

NAMESP_BEGIN
namespace net
{

template<class Socket, class EventQueueT, class MsgWrapper>
class UdpPeer_co : public NetPeer_co<MsgWrapper>
{

    using NetPeer = UdpPeerBasic<Socket, EventQueueT, MsgWrapper>;

public:
	UdpPeer_co(EventQueueT* q, const AddrPair& local_addr)
		:_peer(q, local_addr)
	{
        _peer.open();
        _peer.listenOnRecv([this](const AddrPair& remote, const MsgWrapper& msg){
            this->onRecv(remote, msg);
        });
	}

	UdpPeer_co(EventQueueT* q, unsigned short local_port)
		:UdpPeer_co(q, AddrPair{"", local_port} )
	{
	}

	~UdpPeer_co(){
        close();
	}

	auto async_write(const AddrPair& remote, const MsgWrapper& msg){
        _peer.sendTo(remote, msg);
        return typename NetPeer_co<MsgWrapper>::AwaitableWrite{msg};
    }

	auto async_read(AddrPair& remote){
        return typename NetPeer_co<MsgWrapper>::AwaitableRecv{this, &remote};
    }

	void close(){
        _peer.close();
        this->clear();
    }

	fd_t fd()const { return _peer.fd(); }
	AddrPair local_addr()const { return _peer.local_addr(); }

private:
    NetPeer _peer;

};

}//net
NAMESP_END

#endif /*UDPPEER_CO_BASIC_H*/
