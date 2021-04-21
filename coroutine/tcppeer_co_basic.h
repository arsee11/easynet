///udppeer_co_basic.h

#ifndef	TCPPEER_CO_BASIC_H
#define TCPPEER_CO_BASIC_H

#include "fddef.h"
#include "addr.h"
#include "netpeer.h"
#include "netpeer_co.h"
#include "namespdef.h"
#include <coroutine>
#include <queue>

NAMESP_BEGIN
namespace net
{

template<class Socket, class EventQueueT, class MsgWrapper>
class TcpPeer_co : public NetPeer_co<MsgWrapper>
{

    using NetPeer = NetPeerBasic<Socket, EventQueueT, MsgWrapper>;
    using netpeer_ptr = std::shared_ptr<NetPeer>;

public:
	TcpPeer_co(netpeer_ptr peer)
	    :_peer(peer)
	{
        if(_peer == nullptr){
            return;
        }
        _peer->listenOnRecv([this](const netpeer_ptr&, const MsgWrapper& buf){
            this->onRecv({"",0}, buf);
        });
        _peer->listenOnClose([this](const netpeer_ptr& peer){
            this->close();
        });
	}

	~TcpPeer_co(){
        close();
	}
    
    bool is_valid()const{ return _peer != nullptr; }

	auto async_write(const MsgWrapper& msg){
        if(_peer != nullptr){
            _peer->write(msg);
        }
        return typename NetPeer_co<MsgWrapper>::AwaitableWrite{msg};
    }
	auto async_read(){
        return typename NetPeer_co<MsgWrapper>::AwaitableRecv{this, nullptr};
    }

	void close(){
        _peer->close();
        this->clear();
    }

	fd_t fd()const { return _peer->fd(); }
	AddrPair local_addr()const { return _peer->local_addr(); }
	AddrPair remote_addr()const { return _peer->remote_addr(); }

private:
    netpeer_ptr _peer;

};

}//net
NAMESP_END

#endif /*TCPPEER_CO_BASIC_H*/
