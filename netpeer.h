///netpeer.h
//

#ifndef NET_PEER_H
#define NET_PEER_H

#include "addr.h"
#include "fddef.h"
#include "netevents.h"
#include "event_listener.h"

#include <sys/socket.h>
#include <memory>

NAMESP_BEGIN
namespace net
{
template<class Socket, class EventQueueT, class MsgWrapper>
class NetPeerBasic : 
	public std::enable_shared_from_this<NetPeerBasic<Socket, EventQueueT, MsgWrapper>> 
{
    using NetPeer = NetPeerBasic<Socket, EventQueueT, MsgWrapper>;
    using netpeer_ptr = std::shared_ptr<NetPeer>;
    using InputEvent = NetInputEvent;
    using CloseEvent = NetCloseEvent;
    using OutputEvent = NetOutputEvent;
    using SendCb = std::function<void(int size, int err)>;
    using RecvCb = std::function<void(const netpeer_ptr& peer, const MsgWrapper& buf)>;
    using CloseCb = std::function<void(const netpeer_ptr& peer)>;

public:
    NetPeerBasic()=delete;

    NetPeerBasic(EventQueueT* q, const Socket& socket)
        :_socket(socket)
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
    void listenOnClose(CloseCb call_back){ _close_cb = call_back; }

    int write(const MsgWrapper& msg);
    void write_async(MsgWrapper& msg, SendCb cb);
    void close();

    AddrPair remote_addr()const{ return _socket.remote_addr(); }
    fd_t fd()const{ return _socket.fd(); }

private:
    void onInput();
    void onClose();
    void onOutput(MsgWrapper buf, SendCb call_back);
    int do_write(MsgWrapper& buf, SendCb call_back);

private:
    Socket _socket;
    EventListener<EventQueueT> _listener;
    std::unique_ptr<InputEvent> _input_evt;
    std::unique_ptr<CloseEvent> _close_evt;
    std::unique_ptr<OutputEvent> _output_evt;
    RecvCb _recv_cb;
    CloseCb _close_cb;
};

}//net
NAMESP_END

#include "netpeer.inl"

#endif /*NET_PEER_H*/
