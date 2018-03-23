//reactor.h 
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef REACTOR_H
#define REACTOR_H

#include <algorithm>
#include <list>
#include <iostream>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN
namespace net
{

template<class SELECTOR
>
class Reactor
{
public:
	typedef SELECTOR selector_t; 
	typedef typename selector_t::event_ptr_t event_ptr_t;
	typedef std::list<event_ptr_t > events_t;
	enum {
		EVT_IN = selector_t::EVT_IN,
		EVT_OUT = selector_t::EVT_OUT,
		EVT_CLOSE = selector_t::EVT_CLOSE
	};

public:
	//@HandlerPtr, is a pointer type, smartptr or law ptr.
	template<class HandlerPtr> 
	void RegistryHandler(HandlerPtr& h)
	{
		event_ptr_t pevt = h->FirstEvent();
		for(; pevt != nullptr; pevt=h->NextEvent() )
		{
			_selector.Attach(pevt);	
		}
	}

	template<class HandlerPtr> 
	void RemoveHandle(HandlerPtr& h)
	{
		event_ptr_t pevt = h->FirstEvent();
		for(; pevt != nullptr; pevt=h->NextEvent() )
		{
			_selector.Detach(pevt);
		}
	}

	virtual void EventLoop()
	{
		cout<<"evnet loop..."<<endl;
		while(true)
		{
			events_t act_evts;
			int n =  _selector.Select(act_evts);
			for(auto &i:act_evts)
				i->Notify();
		}
	}

private:
	selector_t _selector;

};

}//net
NAMESP_END

#endif /*REACTOR_H*/
