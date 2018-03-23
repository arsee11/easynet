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
	
#include "netevents.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

///@brief put thd event into filter chain before process,\n
/// if filter return true process will break, \n
/// if return false process continue.
typedef std::function<bool(Event*)> filter_t;


struct FilterHolder 
{
	typedef std::list<filter_t> filter_list_t;
	filter_list_t _netinput_filters;
	filter_list_t _netaccept_filters;
};


bool filterNetInput(const FilterHolder& h, Event* e)
{
	for(auto i : h._netinput_filters)
		if( i(e) )
			return true;

	return false;
}

bool filterNetAccept(const FilterHolder& h, Event* e)
{
	for(auto i : h._netaccept_filters)
		if( i(e) )
			return true;

	return false;
}



//////////////////////////////////////////////////////////////////////
typedef std::unordered_map<std::type_index, std::function<bool(FilterHolder&, Event*)> > event_filter_map_t;

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

}//net
NAMESP_END

#endif /*EVENT_QUEUE_HELPER_H*/
