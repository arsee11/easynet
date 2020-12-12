///event_listener.h
//
#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H
#include <assert.h>

NAMESP_BEGIN
namespace net
{

//////////////////////////////////////////////////////////////////////
///@EventQueue event queue type.
template<class EventQueue>
class EventListener
{
public:
	EventListener(EventQueue* eq):_evt_queue(eq){} 
	
	///@template param E the event
	///@param fd Event fd (id of events)
	template<class E>
    void listen(fd_t fd, E* evt){
		assert(_evt_queue);
		_evt_queue->bind(fd, evt);
	}

    ///@template param E the event
    ///@param fd Event fd (id of events)
	template<class E>
	void unlisten(fd_t fd, E* evt){
		assert(_evt_queue);
    	_evt_queue->unbind(fd, evt);
	}

    ///@param fd Event fd (id of events)
    void unlisten(fd_t fd){
    	assert(_evt_queue);
    	_evt_queue->unbind(fd);
    }

protected:
	EventQueue* _evt_queue=nullptr;
};

}//net
NAMESP_END

#endif /*EVENT_LISTENER_H*/

