///netevent_listener.h
//
#ifndef NETEVENT_LISTENER_HELPER_H
#define NETEVENT_LISTENER_HELPER_H

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <functional>

#include "netevents.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class NetworkEventListener;

typedef std::unordered_map<std::type_index, std::function<void( Event*, NetworkEventListener*)> > event_proc_map_t;

template<class E>
struct Listen;


////////////////////////////////////////////////////////////
void procNetInput(Event* e, NetworkEventListener* l );

template<>
struct Listen<NetInputEvent>
{
	static void add(event_proc_map_t& maps){
		maps.insert( std::make_pair( std::type_index(typeid(NetInputEvent)), procNetInput) );
	}
};

////////////////////////////////////////////////////////////
void procNetAccept(Event* e, NetworkEventListener* l );

template<>
struct Listen<NetAcceptEvent>
{
	static void add(event_proc_map_t& maps){
		maps.insert( std::make_pair( std::type_index(typeid(NetAcceptEvent)), procNetAccept) );
	}
};

////////////////////////////////////////////////////////////
void procNetClose(Event* e, NetworkEventListener* l );

template<>
struct Listen<NetCloseEvent>
{
	static void add(event_proc_map_t& maps){
		maps.insert( std::make_pair( std::type_index(typeid(NetCloseEvent)), procNetClose) );
	}
};

}//net
NAMESP_END

#endif /*NETEVENT_LISTENER_HELPER_H*/
