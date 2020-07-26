///event_listener_basic.h
//
#ifndef EVENT_LISTENER_BASIC_H
#define EVENT_LISTENER_BASIC_H
#include <typeindex>
#include "event_listener.h"
#include <list>
#include <assert.h>

NAMESP_BEGIN
namespace net
{




//////////////////////////////////////////////////////////////////////
///@EventQueue event queue type.
///@param EventHandler callable object with like EventHandler(Params...).
///@param Params... the params past to the EventHandler.
template<class EventQueue, class EventHandler, typename... Params>
class EventListenerBasic : public EventListener
{

public:
	using evthandler_t = EventHandler;

public:
	EventListenerBasic(EventQueue* eq):_evt_queue(eq){} 
	
	///@template param E the event
	///@param fd Event fd (id of events)
	template<class E>
	void listen(fd_t fd, E* evt, const EventHandler& h){
		assert(_evt_queue);
		evt->addListener(this);
		_evt_queue->bind(fd, evt);
		_evt_handler = h;
	}

	template<class E>
	void unlisten(fd_t fd, E* evt){
		assert(_evt_queue);
		evt->delListener(this);
		_evt_queue->unbind(fd);
	}

	void operator()(Params... params)const{
		_evt_handler(params...);	
	}	

protected:
	EventQueue* _evt_queue=nullptr;
	EventHandler _evt_handler;

};

}//net
NAMESP_END

#endif /*EVENT_LISTENER_BASIC_H*/

