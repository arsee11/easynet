///event.h

#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include "namespdef.h"
#include "event_listener.h"
#include <list>
#include <algorithm>

NAMESP_BEGIN
namespace net
{


//////////////////////////////////////////////////////////////////////////
class Event
{
public:
	virtual ~Event()
	{
	}

	virtual void fire()=0;

};

using event_ptr = Event*;

///@param I index 1,2,3,...,n
///@param Func funtion with n args
///@param Tuple std::tuple instance
template<size_t... I, typename Func, typename Tuple>
void call_tuple_arg(const Func& f, const Tuple& t){
	f( std::get<I>(t)... );
}


template<class Listener, class... Params>
class EventBasic: public Event
{
public:
	using listener_t = Listener;
	using listener_ptr = listener_t*;
	using listener_list = std::list<listener_ptr>;

public:
	EventBasic(Params... params){
		_tuple = std::make_tuple(params...);
	}

	void addListener(listener_ptr l){ _listeners.push_back(l); }

	void delListener(listener_ptr l){ 
		auto i = std::find(_listeners.begin(), _listeners.end(), l);
		if(i != _listeners.end())
			_listeners.push_back(l);
       	}

protected:

	template<int... I>
	void call(){
		for(auto& i : this->_listeners)
		{
			listener_ptr l = static_cast<listener_ptr>(i);
			call_tuple_arg<I...>(*l, this->_tuple);
		}
	}
	
	void call(){
		for(auto& i : this->_listeners)
		{
			i->operator()();
		}
	}
	

protected:
	listener_list _listeners;
	std::tuple<Params...> _tuple;
};

}//net
NAMESP_END

#endif /*EVENT_H*/
