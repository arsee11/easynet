//acceptor_completed.h

#ifndef ACCEPTOR_COMPLETED_H
#define ACCEPTOR_COMPLETED_H

#include "addr.h"
#include "netevents.h"
#include "pollevents.h"
#include "namespdef.h"

#include <functional>
#include <memory>

NAMESP_BEGIN
namespace net
{

template<class EventQueueT>
using PollInputListner = typename PollInputEvent<EventQueueT>::listener_t;

//@param EventQueueT
//@param AcceptorBasicT 
//@param NetPeer
template<class EventQueueT, class AcceptorBasicT
	,class NetPeer, class NetPeerPtr
>
class AcceptorCompleted: public PollInputListner<EventQueueT>, public AcceptorBasicT
{ 
	using Event = PollInputEvent<EventQueueT>;
	using AcceptEvent = AcceptCompletedEvent<EventQueueT, NetPeerPtr>;

public:
	AcceptorCompleted(EventQueueT* q, const AddrPair& local_addr)
		:PollInputListner<EventQueueT>(q)
		,AcceptorBasicT(q, local_addr)
	{
		this->template open();
		this->template listen<Event>(this->template fd(), [this](fd_t fd){ this->onAccept(fd); } );
	}

	AcceptorCompleted(EventQueueT* q, unsigned short port)
		:PollInputListner<EventQueueT>(q)
		,AcceptorBasicT(q, port)
	{
		this->template open();
		this->template listen<Event>(this->template fd(), [this](fd_t fd){ this->onAccept(fd); } );
	}

	~AcceptorCompleted(){
		close();
	}

	void close(){
		AcceptorBasicT::close();
		this->template unlisten<Event>(this->template fd());
	}

private:
	void onAccept(fd_t fd){
		sockaddr_in addr;
		socklen_t len = sizeof(addr);	
		fd_t newfd = ::accept(fd, (sockaddr*)&addr, &len);
		AcceptorBasicT::_evt_queue->attach(newfd);
		NetPeerPtr peer( new NetPeer(AcceptorBasicT::_evt_queue, newfd, makeAddrPair(&addr)) );
		if(AcceptorBasicT::_evt_queue != nullptr)
		{
			const listener_list& llist = AcceptorBasicT::_evt_queue->findListenerList(
				std::type_index(typeid(AcceptEvent))
			);
			AcceptorBasicT::_evt_queue->pushEvent(std::make_shared<AcceptEvent>(llist, peer));
		}
	}

};

}//net
NAMESP_END

#endif /*ACCEPTOR_COMPLETED_H*/
