//acceptor_completed.h

#ifndef ACCEPTOR_COMPLETED_H
#define ACCEPTOR_COMPLETED_H

#include "addr.h"
#include "netevents.h"
#include "event_listener.h"
#include "namespdef.h"

#include <functional>

NAMESP_BEGIN
namespace net
{


///call back when accpet event was fired
///@param 1 netpeer ptr was accepted.
template<class NetPeerPtr>
using AcceptCb_c = std::function<void(NetPeerPtr)>;

//@param EventQueueT
//@param NetPeer
template<class Socket, class EventQueueT
	,class NetPeer, class NetPeerPtr
>
class AcceptorCompleted: public EventListener<EventQueueT>
		,public AcceptorBasic<Socket, NetInputEvent, AcceptCb_c<NetPeerPtr>>
{ 
	using AcceptorBase=AcceptorBasic<Socket, NetInputEvent, AcceptCb_c<NetPeerPtr>>;
public:
	AcceptorCompleted(EventQueueT* q, const AddrPair& local_addr)
		:EventListener<EventQueueT>(q)
		,AcceptorBase(local_addr)
	{
		this->_evt.reset( new typename AcceptorBase::event_t([this](){this->onInput();}) );
		this->template listen(this->template fd(), this->_evt.get());
	}

	AcceptorCompleted(EventQueueT* q, unsigned short port)
		:AcceptorCompleted(q, AddrPair{"", port})
	{
	}

	~AcceptorCompleted(){
		this->template unlisten(this->template fd());
	}

private:
	void onInput(){
		Socket cli = this->_socket.accept();
        if(cli.invalid()){
            //log 
            return;
        }
		NetPeerPtr peer( new NetPeer(this->_evt_queue, cli) );
		if(this->_accept_cb != nullptr){
			this->_accept_cb(peer);
		}
	}
};

}//net
NAMESP_END

#endif /*ACCEPTOR_COMPLETED_H*/
