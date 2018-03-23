///proactor.h 

#ifndef PROACTOR_H
#define PROACTOR_H


#include "netevents.h"
#include "netpeer.h"
#include "event_queue.h"
#include "acceptor.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

//////////////////////////////////////////////////////////////////////////////
template<class Selector>
class Proactor
{
	typedef Proactor<Selector> my_t;

	static constexpr int MAX_BUF_LEN = 65535;
	
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
	bool filterNetInput(Event* e){
		onInput(e);	
		return true;
	}

	bool filterNetAccept(Event* e){
		onAccept(e);	
		return true;
	}
	
	void onInput(Event* e){
		netpeer_ptr_t peer = _netpeer_holder[e->sender()->fd()];
		delete e->sender();
		char* buf = (char*)malloc(MAX_BUF_LEN);
		int rlen = peer->read(buf, MAX_BUF_LEN);
		if( rlen > 0 )
		{
			Event* ne = new NetInputEvent(peer.get()); 
			ne->extra(buf, rlen);
			_event_queue->process(ne);	
		}
		else
		{
			free(buf);
			_selector.detach(peer->fd());
			peer->close();
			Event* ne = new NetCloseEvent(peer.get()); 
			_event_queue->process(ne);	
			_netpeer_holder.erase(peer->fd());
		}
	}

	void onAccept(Event* e){
		NetPeer* peer = _acceptor->accept();
		_netpeer_holder.insert( std::make_pair(peer->fd(), netpeer_ptr_t(peer)) );
		_selector.attach(peer->fd());
		Event* ne = new NetAcceptEvent(peer);
		if(ne != nullptr)
		{
			_event_queue->process(ne);	
		}
		else
		{
		}
	}
	
protected:
	Selector _selector;
	EventQueue* _event_queue;	
	Acceptor* _acceptor = nullptr;
	netpeer_manager_t _netpeer_holder;
};

}//net
NAMESP_END

#endif /*PROACTOR_H*/
