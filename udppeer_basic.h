///udppeer_basic.h

#ifndef	UDPPEER_BASIC_H 
#define UDPPEER_BASIC_H 

#include "fddef.h"
#include "addr.h"
#include "netevents.h"
#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

template<class Socket, class EventQueueT, class MsgWrapper>
class UdpPeerBasic :public EventListener<EventQueueT>
{
	using OnRecvCb = std::function<void(const AddrPair& local_addr, const MsgWrapper& msg)>;

	using InputEvent = NetInputEvent;

public:
	UdpPeerBasic(EventQueueT* q, const AddrPair& local_addr)
		:EventListener<EventQueueT>(q)
		,_socket(local_addr)
	{
	}

	UdpPeerBasic(EventQueueT* q, unsigned short local_port)
		:UdpPeerBasic(q, AddrPair{"", local_port} )
	{
	}

	~UdpPeerBasic(){
        if(this->fd() != INVALID_SOCKET){
            this->close();
        }
	}

	void open();

	void close();

	void sendTo(const AddrPair& remote, const MsgWrapper& msg);

	fd_t fd()const { return _socket.fd(); }
	AddrPair local_addr()const{ return _socket.local_addr(); }
	
	void listenOnRecv(const OnRecvCb& cb){ _onrecv_cb=cb; }

private:
	void onInput();

private:
    Socket _socket;
	std::unique_ptr<InputEvent> _evt;
	OnRecvCb _onrecv_cb;

};

}//net
NAMESP_END

#include "udppeer_basic.inl"

#endif /*UDPPEER_BASIC_H*/
