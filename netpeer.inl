///netpeer.inl
//

#include <sys/types.h>
#include <sys/socket.h>
#include "event_queue_helper.h"
	
NAMESP_BEGIN
namespace net
{

template<class EventQueueT, class MsgWrapper>
void NetPeerBasic<EventQueueT,  MsgWrapper>::onInput(fd_t fd)
{
	MsgWrapper msg(10240);
	int rsize=read(msg.begin(), msg.maxsize());
	msg.size(rsize);

	if(rsize > 0 )
	{
		const listener_map& lmap= _evt_queue->findListenerMap(
			std::type_index(typeid(CompletedEvent))
		);

		if(lmap.size() > 0 )
		{
			auto l=lmap.find(fd);
			if( l!= lmap.end())
				_evt_queue->pushEvent( event_ptr(
					new CompletedEvent(l->second, this->shared_from_this(), msg ) ) );
		}	
	}
	else
	{
		_evt_queue->detach(fd);
		const listener_map& lmap= _evt_queue->findListenerMap(
			std::type_index(typeid(CloseEvent))
		);

		if(lmap.size() > 0 )
		{
			auto l=lmap.find(fd);
			if( l!= lmap.end())
				_evt_queue->pushEvent( event_ptr(
					new CloseEvent(l->second, this->shared_from_this()) ));
		}	

		this->close();
	}
	
}

template<class EventQueueT, class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::write(const MsgWrapper& msg)
{
	return write(msg.begin(), msg.size());
}

template<class EventQueueT, class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::write(const void *buf, size_t len)
{
	size_t n=0;
	do
	{
		int m = send(_fd, (const char*)buf+n, len-n, 0);
		if(m < 0 )
			break;

		n += m;
	}while( n < len );

	return n;
}

template<class EventQueueT,  class MsgWrapper>
void NetPeerBasic<EventQueueT,  MsgWrapper>::close()
{
	if(_fd != INVALID_SOCKET)
	{
		
		this->template unlisten<Event>(this->template fd());
		::close(_fd);
	}
}

template<class EventQueueT,  class MsgWrapper>
int NetPeerBasic<EventQueueT,  MsgWrapper>::read(void *buf, int len)
{
	return ::recv(_fd, buf, len, 0);
}

}//net
NAMESP_END
