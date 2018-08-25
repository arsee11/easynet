///netpeer.h
//

#ifndef NET_PEER_H
#define NET_PEER_H

#include "addr.h"
#include "fddef.h"
#include "pollevents.h"

#include <sys/socket.h>
#include <memory>

NAMESP_BEGIN
namespace net
{

template<class EventQueueT>
using PollListner = typename PollInputEvent<EventQueueT>::listener_t;

template<class EventQueueT, class MsgWrapper>
class NetPeerBasic : public PollListner<EventQueueT>
	,public std::enable_shared_from_this<NetPeerBasic<EventQueueT, MsgWrapper>> 
{
	using NetPeer = NetPeerBasic<EventQueueT, MsgWrapper>;
	using netpeer_ptr = std::shared_ptr<NetPeer>;
	using Event = PollInputEvent<EventQueueT>;
	using CompletedEvent = InputCompletedEvent<EventQueueT, netpeer_ptr, MsgWrapper>;
	using CloseEvent = CloseCompletedEvent<EventQueueT, netpeer_ptr>;

public:
	NetPeerBasic()=delete;

	NetPeerBasic(EventQueueT* q, fd_t fd, const AddrPair& remote_addr)
		:PollListner<EventQueueT>(q)
		,_remote_addr(remote_addr)
		,_fd(fd)
		,_evt_queue(q)
	{
		this->template listen<Event>(this->template fd(), [this](fd_t fd){ this->onInput(fd); } );
	}

	~NetPeerBasic(){ close(); }

	AddrPair remote_addr(){ return _remote_addr; }
	
	void close();
	fd_t fd()const{ return _fd; }

	int write(const MsgWrapper& msg);

private:
	void onInput(fd_t);
	int read(void *buf, int len);
	int write(const void* buf, size_t len);

private:
	AddrPair _remote_addr;
	fd_t _fd;
	EventQueueT* _evt_queue=nullptr;
};

}//net
NAMESP_END

#include "netpeer.inl"

#endif /*NET_PEER_H*/
