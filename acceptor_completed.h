//acceptor_completed.h

#ifndef ACCEPTOR_COMPLETED_H
#define ACCEPTOR_COMPLETED_H

#include "addr.h"
#include "netevents.h"
#include "namespdef.h"

#include <functional>

NAMESP_BEGIN
namespace net
{

template<class EventQueueT>
using CAcceptListener = typename NetAcceptEvent<EventQueueT>::listener_t;

///call back when accpet event was fired
///@param 1 netpeer ptr was accepted.
template<class NetPeerPtr>
using AcceptCb_c = std::function<void(NetPeerPtr)>;

//@param EventQueueT
//@param AcceptorBasicT 
//@param NetPeer
template<class EventQueueT
	,class NetPeer, class NetPeerPtr
>
class AcceptorCompleted: public CAcceptListener<EventQueueT>
		       , public AcceptorBasic<NetAcceptEvent<EventQueueT>
				             ,AcceptCb_c<NetPeerPtr>>
{ 
	using AcceptorBase=AcceptorBasic<NetAcceptEvent<EventQueueT>
				        ,AcceptCb_c<NetPeerPtr>>;
public:
	AcceptorCompleted(EventQueueT* q, const AddrPair& local_addr)
		:CAcceptListener<EventQueueT>(q)
		,AcceptorBase(local_addr)
	{
		this->_evt.reset( new typename AcceptorBase::event_t() );
		this->template listen(this->template fd(), this->_evt.get(),
				[this](){ this->onInput();});
	}

	AcceptorCompleted(EventQueueT* q, unsigned short port)
		:AcceptorCompleted(q, AddrPair{port, ""})
	{
	}

	~AcceptorCompleted(){
		this->template unlisten(this->template fd(), this->_evt.get());
	}

private:
	void onInput(){
		sockaddr_in addr;
		socklen_t len = sizeof(addr);	
		fd_t newfd = ::accept(this->template fd(), (sockaddr*)&addr, &len);
		NetPeerPtr peer( new NetPeer(this->_evt_queue, newfd, makeAddrPair(&addr)) );
		if(this->_accept_cb != nullptr){
			this->_accept_cb(peer);
		}
	}
};

}//net
NAMESP_END

#endif /*ACCEPTOR_COMPLETED_H*/
