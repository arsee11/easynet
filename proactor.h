///proactor.h 

#ifndef PROACTOR_H
#define PROACTOR_H


#include "netevents.h"
#include "netpeer.h"
#include "event_queue.h"

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#include "acceptor.h"
#include "acceptor_udp.h"

NAMESP_BEGIN
namespace net
{

//////////////////////////////////////////////////////////////////////////////
template<class Selector, class Acceptor>
class Proactor
{
	typedef Proactor<Selector, Acceptor> my_t;

	static constexpr int MAX_BUF_LEN = 1400*8;
	
public:
	void run()
	{
		typename Selector::event_set_t events =  _selector.select();
		if( events.size() == 0 )
			return;
		
		for(auto e : events)
	        	_event_queue->pushEvent(e);	
		
		_event_queue->process();
	}

	void setAcceptor(Acceptor* a){
		_selector.listenfd(a->fd());
		_acceptor = a;
	}
	
	Proactor(EventQueue* eq):_event_queue(eq){
		using namespace std::placeholders;
		_event_queue->addFilter<NetInputEvent>( std::bind(&my_t::filterNetInput, this, _1) );
		_event_queue->addFilter<NetAcceptEvent>( std::bind(&my_t::filterNetAccept, this, _1) );
	}
	
private:
	//return true if want to break the event process.
	bool filterNetInput(Event* e){
		onInput(e);	
		return true;
	}

	//return true if want to break the event process.
	bool filterNetAccept(Event* e){
		onAccept(e);	
		return true;
	}
	
	void onInput(Event* e){
		netpeer_ptr_t peer = _acceptor->getPeer( 
			*static_cast<typename Acceptor::netpeer_key_t*>(e->sender()) 
		);

		int rlen = peer->read(_recvbuf, MAX_BUF_LEN);
		if( rlen > 0 )
		{
			Event* ne = new NetInputEvent(peer.get()); 
			ne->extra(_recvbuf, rlen);
			_event_queue->process(ne);	
		}
		else
		{
			_selector.detach(peer->fd());
			peer->close();
			Event* ne = new NetCloseEvent(peer.get()); 
			_event_queue->process(ne);	
		}
	}

	void onAccept(Event* e){
		Event* ne=nullptr;
		NetPeer* peer=nullptr;
		std::tie(ne, peer)= _acceptor->accept();
		if(peer != nullptr) 
		{
			_selector.attach(peer->fd());
		}
		if(ne != nullptr)
		{
			_event_queue->process(ne);	
		}
		else
		{
		}
	}
	
protected:
	char _recvbuf[MAX_BUF_LEN];
	Selector _selector;
	EventQueue* _event_queue;	
	Acceptor* _acceptor = nullptr;
};


template<class Selector>
using ProactorUdp = Proactor<Selector, AcceptorUdp>;

template<class Selector>
using ProactorTcp = Proactor<Selector, Acceptor>;

}//net
NAMESP_END

#endif /*PROACTOR_H*/
