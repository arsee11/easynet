//acceptor_reactor.h

#ifndef ACCEPTOR_REACTOR_H
#define ACCEPTOR_REACTOR_H

#include "addr.h"
#include "acceptor_basic.h"
#include "netevents.h"
#include "namespdef.h"

#include <functional>

NAMESP_BEGIN
namespace net
{


template<class EventQueueT>
using AcceptListener = typename NetAcceptEvent<EventQueueT>::listener_t;

///call back when accpet event was fired
///@param fd the acceptable fd, that can call ::accept socket function on it 
///later.
using AcceptCb_r = std::function<void(fd_t fd)>;

template<class EventQueueT>
class AcceptorReactor: public AcceptListener<EventQueueT>
		     , public AcceptorBasic<NetAcceptEvent<EventQueueT>, AcceptCb_r>
{ 

	using AcceptorBase=AcceptorBasic<NetAcceptEvent<EventQueueT>, AcceptCb_r>;
public:
	AcceptorReactor(EventQueueT* q, const AddrPair& local_addr)
		:AcceptListener<EventQueueT>(q)
		,AcceptorBase(local_addr)
	{
		this->_evt.reset( new typename AcceptorBase::event_t() );
		this->template listen(this->template fd(), this->_evt.get()
				, [this](){ this->onInput(); } );
	}

	AcceptorReactor(EventQueueT* q, unsigned short port)
		:AcceptorReactor(q, AddrPair{port, ""})
	{
	}
	
	~AcceptorReactor(){
		this->template unlisten(this->template fd(), this->_evt.get());
	}

private:
	void onInput(){
		if(this->_accept_cb != nullptr){
			this->_accept_cb(this->template fd() );
		}
	}

};

}//net
NAMESP_END

#endif /*ACCEPTOR_BASIC_H*/
