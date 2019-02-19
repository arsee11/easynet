///netevent_queue_basic.h
//
//

#ifndef NETEVENT_QUEUE_BASIC_H
#define NETEVENT_QUEUE_BASIC_H

#include <memory>
#include "epoll.h"

#include <iostream>

NAMESP_BEGIN
namespace net
{

template<class EventQueueT>
class NetEventQueueBasic : public EventQueueT
{ 
	using Poller = Epoll<NetEventQueueBasic<EventQueueT>>;
public:
	NetEventQueueBasic()
	{
		_poller.reset( new Poller(this) );
	}

	~NetEventQueueBasic()
	{
		std::cout<<"~NetEventQueueBasic()"<<std::endl;
	}

        void process(){
		_poller->select();
		EventQueue::process();
	}

	void attach(fd_t fd){ _poller->attach(fd); }
	void detach(fd_t fd){ _poller->detach(fd); }

private:
	std::unique_ptr<Poller> _poller;
};

}//net
NAMESP_END

#endif /*NETEVENT_QUEUE_BASIC_H*/
