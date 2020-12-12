///netpeer.h
//

#ifndef NET_PEER_H
#define NET_PEER_H

#include "addr.h"
#include "fddef.h"
#include "event.h"
#include "event_listener.h"

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
	using InputEvent = NetInputEvent;
	using CloseEvent = NetCloseEvent;
    using OutputEvent = NetOutputEvent;
    using SendCb = std::function<void(int size, int err)>;
    using RecvCb = std::function<void(const netpeer_ptr& peer, const MsgWrapper& buf)>;

public:
	NetPeerBasic()=delete;

	NetPeerBasic(EventQueueT* q, fd_t fd, const AddrPair& remote_addr)
		:_remote_addr(remote_addr)
		,_fd(fd)
        ,_listener(q)
	{
		_input_evt.reset(new InputEvent([this](){this->onInput();}) );
		_listener.listen(this->fd(), _input_evt.get());

		_output_evt.reset(new OutputEvent(nullptr));

		_close_evt.reset(new CloseEvent( [this](){this->onClose();}) );
		_listener.listen(this->fd(), _close_evt.get());
	}

	~NetPeerBasic(){ close(); }

	
    void listenOnRecv(RecvCb call_back){ _recv_cb = call_back; }

	int write(const MsgWrapper& msg);
    void write_async(MsgWrapper& msg, SendCb cb);
	int read(void *buf, int len);

    void close();
	AddrPair remote_addr(){ return _remote_addr; }
	fd_t fd()const{ return _fd; }


private:
	void onInput();
	void onClose();
    void onOutput(MsgWrapper buf, SendCb call_back);
    int do_write(MsgWrapper& buf, SendCb call_back);

private:
	AddrPair _remote_addr;
	fd_t _fd;
    EventListener<EventQueueT> _listener;
	std::unique_ptr<InputEvent> _input_evt;
	std::unique_ptr<CloseEvent> _close_evt;
    std::unique_ptr<OutputEvent> _output_evt;
    RecvCb _recv_cb;

};

}//net
NAMESP_END

#include "netpeer.inl"

#endif /*NET_PEER_H*/
