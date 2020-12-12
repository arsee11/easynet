//acceptor_reactor.h

#ifndef ACCEPTOR_REACTOR_H
#define ACCEPTOR_REACTOR_H

#include "addr.h"
#include "acceptor_basic.h"
#include "netevents.h"
#include "event_listener.h"
#include "namespdef.h"

#include <functional>

NAMESP_BEGIN
namespace net
{

///call back when accpet event was fired
///@param fd the acceptable fd, that can call ::accept socket function on it 
///later.
using AcceptCb_r = std::function<void(fd_t fd)>;

template<class EventQueueT>
class AcceptorReactor: public EventListener<EventQueueT>
		     , public AcceptorBasic<NetInputEvent, AcceptCb_r>
{ 

	using AcceptorBase=AcceptorBasic<NetInputEvent, AcceptCb_r>;
public:
	AcceptorReactor(EventQueueT* q, const AddrPair& local_addr)
		:EventListener<EventQueueT>(q)
		,AcceptorBase(local_addr)
	{
		this->_evt.reset( new typename AcceptorBase::event_t([this](){this->onInput();}) );
		this->template listen(this->fd(), this->_evt.get());
	}

	AcceptorReactor(EventQueueT* q, unsigned short port)
		:AcceptorReactor(q, AddrPair{port, ""})
	{
	}
	
	~AcceptorReactor(){
		this->template unlisten(this->fd());
	}

private:
	void onInput(){
		if(this->_accept_cb != nullptr){
			this->_accept_cb(this->fd() );
		}
	}

};

}//net
NAMESP_END

#endif /*ACCEPTOR_BASIC_H*/
