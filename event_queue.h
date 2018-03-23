///event_queue.h
//
//

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "event_queue_helper.h"	
#include "event_listener.h"	

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

///@breif take onwership of Event.
class EventQueue
{
public:
	template<class E>
	void addFilter(const filter_t& f){
		AddHandler<E>::addFilter(_event_filter_map, _filters, f);		
	
	}

	void addListener(EventListener* l){
		_listeners.push_back(l);			
		
	}

	void pushEvent(Event* e){
		_events.push_back(e);
	}


	void process(){
		auto i = _events.begin();
		for(; i != _events.end();)
		{
			Event *e = *i;
			i = _events.erase(i);
			if(filter(e))
			{
				delete e;
				return;
			}

			process(e);
		}

	}

	void process(Event* e){
		for(auto l : _listeners)
			l->onEvent(e);

		delete e;
	}

private:
	bool filter(Event* e){
		auto i = _event_filter_map.find( std::type_index( typeid(*e)) );
		if( i != _event_filter_map.end() )
			return i->second(_filters, e);

		return false;
	}

private:
	event_filter_map_t _event_filter_map;

	typedef std::list<Event*> event_list_t;
	event_list_t _events;

	typedef std::list<EventListener*> event_listener_list_t;
	event_listener_list_t _listeners;
	FilterHolder _filters;
};

}//net
NAMESP_END

#endif /*EVENT_QUEUE_H*/
