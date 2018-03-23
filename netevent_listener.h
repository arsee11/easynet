///netevent_listener.h
//
#ifndef NETEVENT_LISTENER_H
#define NETEVENT_LISTENER_H

#include "netevent_listener_helper.h"
#include "event_listener.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class NetPeer;

/////////////////////////////////////////////////////////////////////////
class NetworkEventListener : public EventListener
{

public:
	virtual void onInput(const void* buf, size_t len, NetPeer* conn)=0;
	virtual void onNewPeer(NetPeer* conn)=0;
	virtual void onOutput(NetPeer* conn)=0;
	virtual void onClose(NetPeer* conn)=0;
	
	template<class E>
	void listen(){
		Listen<E>::add(_event_proc_map);
	}

private:
	void onEvent(Event* e)override{
		auto i = _event_proc_map.find( std::type_index( typeid(*e)) );
		if( i != _event_proc_map.end() )
			i->second(e, this);
	}	


	event_proc_map_t _event_proc_map;
};

}//net
NAMESP_END

#endif /*NETEVENT_LISTENER_H*/
