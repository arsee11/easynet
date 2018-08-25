///event_queue_helper.h
//
//

#ifndef EVENT_QUEUE_HELPER_H
#define EVENT_QUEUE_HELPER_H

#include <list>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <functional>
	
//#include "netevents.h"

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{
/*
///@brief put thd event into filter chain before process,\n
/// if filter return true process will break, \n
/// if return false process continue.
typedef std::function<bool(event_ptr)> filter_t;


struct FilterHolder 
{
	typedef std::list<filter_t> filter_list_t;
	filter_list_t _netinput_filters;
	filter_list_t _netaccept_filters;
};


inline bool filterNetInput(const FilterHolder& h, event_ptr e)
{
	for(auto i : h._netinput_filters)
		if( i(e) )
			return true;

	return false;
}

inline bool filterNetAccept(const FilterHolder& h, event_ptr e)
{
	for(auto i : h._netaccept_filters)
		if( i(e) )
			return true;

	return false;
}



//////////////////////////////////////////////////////////////////////
typedef std::unordered_map<std::type_index, std::function<bool(FilterHolder&, event_ptr)> > event_filter_map_t;

template<class E>
struct AddHandler;

template<>
struct AddHandler<NetInputEvent>
{
	static void addFilter(event_filter_map_t& maps, FilterHolder& h, const filter_t& f){
		h._netinput_filters.push_back(f);
		maps.insert( std::make_pair(std::type_index(typeid(NetInputEvent)), filterNetInput) );
	}
};


template<>
struct AddHandler<NetAcceptEvent>
{
	static void addFilter(event_filter_map_t& maps, FilterHolder& h, const filter_t& f){
		h._netaccept_filters.push_back(f);
		maps.insert( std::make_pair(std::type_index(typeid(NetAcceptEvent)), filterNetAccept) );
	}
};
*/

template<class EventT, class EventQueueT>
void postEventMap(Event* evt, EventQueueT* eq, fd_t fd)
{
	const listener_map& lmap= eq->findListenerMap(
		std::type_index(typeid(EventT))
	);

	if(lmap.size() > 0 )
	{
		auto l=lmap.find(fd);
		if( l!= lmap.end())
			eq->pushEvent( event_ptr(evt) );
	}	
}

}//net
NAMESP_END

#endif /*EVENT_QUEUE_HELPER_H*/
