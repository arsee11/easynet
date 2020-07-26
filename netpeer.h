///netpeer.h
//

#ifndef NET_PEER_H
#define NET_PEER_H

#include "addr.h"
#include "fddef.h"
#include "netevents.h"

#include <sys/socket.h>
#include <memory>

NAMESP_BEGIN
namespace net
{
template<class EventQueueT, class MsgWrapper>
class NetPeerBasic : 
	public std::enable_shared_from_this<NetPeerBasic<EventQueueT, MsgWrapper>> 
{
	using NetPeer = NetPeerBasic<EventQueueT, MsgWrapper>;
	using netpeer_ptr = std::shared_ptr<NetPeer>;
	using InputEvent = NetInputEvent<EventQueueT>;
	using CloseEvent = NetCloseEvent<EventQueueT>;
	using InputListener = typename InputEvent::listener_t;
	using CloseListener = typename CloseEvent::listener_t;

public:
	NetPeerBasic()=delete;

	NetPeerBasic(EventQueueT* q, fd_t fd, const AddrPair& remote_addr)
		:_remote_addr(remote_addr)
		,_fd(fd)
		,_evt_queue(q)
		,_inl(q)
		,_closel(q)
	{
		_input_evt.reset(new InputEvent() );
		_inl.listen(this->fd(), _input_evt.get(), [this](){this->onInput();});
		_close_evt.reset(new CloseEvent() );
		_closel.listen(this->fd(), _close_evt.get(), [this](){this->onClose();});
	}

	~NetPeerBasic(){ close(); }

	AddrPair remote_addr(){ return _remote_addr; }
	
	void close();
	fd_t fd()const{ return _fd; }

	int write(const MsgWrapper& msg);

private:
	void onInput();
	void onClose();
	int read(void *buf, int len);
	int write(const void* buf, size_t len);

private:
	AddrPair _remote_addr;
	fd_t _fd;
	EventQueueT* _evt_queue=nullptr;
	InputListener _inl;
	CloseListener _closel;
	std::unique_ptr<InputEvent> _input_evt;
	std::unique_ptr<CloseEvent> _close_evt;
};

}//net
NAMESP_END

#include "netpeer.inl"

#endif /*NET_PEER_H*/
