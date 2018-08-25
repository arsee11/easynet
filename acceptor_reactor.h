//acceptor_reactor.h

#ifndef ACCEPTOR_REACTOR_H
#define ACCEPTOR_REACTOR_H

#include "addr.h"
#include "event_listener_basic.h"
#include "pollevents.h"
#include "namespdef.h"

#include <functional>
#include <typeindex>

NAMESP_BEGIN
namespace net
{


template<class EventQueueT>
using PollInputListner = typename PollInputEvent<EventQueueT>::listener_t;

template<class EventQueueT, class AcceptorBasicT>
class AcceptorReactor: public PollInputListner<EventQueueT>, public AcceptorBasicT
{ 
	using Event = PollInputEvent<EventQueueT>;
	using AcceptEvent = NetAcceptEvent<EventQueueT>;

public:
	AcceptorReactor(EventQueueT* q, const AddrPair& local_addr)
		:PollInputListner<EventQueueT>(q)
		,AcceptorBasicT(q, local_addr)
	{
		this->template open();
		this->template listen<Event>(this->template fd(), [this](fd_t fd){ this->onInput(fd); } );
		
	}

	AcceptorReactor(EventQueueT* q, unsigned short port)
		:PollInputListner<EventQueueT>(q)
		,AcceptorBasicT(q, port)
	{
		this->template open();
		this->template listen<Event>(this->template fd(), [this](fd_t fd){ this->onInput(fd); } );
	}
	
	~AcceptorReactor(){
		close();
	}

	void close(){
		AcceptorBasicT::close();
		this->template unlisten<Event>(this->template fd());
	}


	void onInput(fd_t fd)
	{
		const listener_list& llist = AcceptorBasicT::_evt_queue->findListenerList(
			std::type_index(typeid(AcceptEvent))
		);
		AcceptorBasicT::_evt_queue->pushEvent( event_ptr(new AcceptEvent(llist, this->template fd())) );
	}
	
};

}//net
NAMESP_END

#endif /*ACCEPTOR_BASIC_H*/
