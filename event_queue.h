///event_queue.h
//
//

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "event.h"
#include "event_listener.h"
#include <deque>
#include <unordered_map>

#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

///@breif take onwership of Event.
class EventQueue
{ 
public:
/*	template<class E>
	void addFilter(const filter_t& f){
		AddHandler<E>::addFilter(_event_filter_map, _filters, f);		
	}
*/

	void pushEvent(const event_ptr& e){ _events.push_back(e); }

	inline void bind(std::type_index evttype, EventListener* l); 
	inline void unbind(std::type_index evttype, EventListener* l ); 
	inline const listener_list& findListenerList(std::type_index evttype)const{ 
		const auto& i = _type_listener_map.find(evttype);
		if(i!=_type_listener_map.end())
			return i->second;

		return _empty_list;
	}

	inline void bindfd(std::type_index evttype, fd_t fd, EventListener* l); 
	inline void unbindfd(std::type_index evttype, fd_t fd); 
	inline const listener_map& findListenerMap(std::type_index evttype)const{ 
		const auto& i = _fd_listener_map.find(evttype);
		if(i!=_fd_listener_map.end())
			return i->second;

		return _empty_map;
	}

	inline void process();
	void process(event_ptr& e){ e->fire();}

private:
	//return true if want to break the event process.
//	inline bool filter(event_ptr e);
private:
//	FilterHolder _filters;
//	event_filter_map_t _event_filter_map;

	typedef std::deque<event_ptr> event_queue;
	event_queue _events;

	typedef std::unordered_map<std::type_index, listener_list> event_type_listener_map;
	event_type_listener_map _type_listener_map;
	
	typedef std::unordered_map<std::type_index, listener_map> event_fd_listener_map;
	event_fd_listener_map _fd_listener_map;

	listener_list _empty_list;
	listener_map _empty_map;
};

}//net
NAMESP_END

#include "event_queue.inl"

#endif /*EVENT_QUEUE_H*/
