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

template<class EventQueueT, class MsgWrapper>
class UdpPeerBasic 
{
	using OnRecvCb = std::function<void(const AddrPair& local_addr, const MsgWrapper& msg)>;

	using InputEvent = NetInputEvent<EventQueueT>;
	using InputListner = typename InputEvent::listener_t;

public:
	UdpPeerBasic(EventQueueT* q, const AddrPair& local_addr)
		:_local_addr(local_addr)
		,_listener(q)
	{
	}

	UdpPeerBasic(EventQueueT* q, unsigned short local_port)
		:UdpPeerBasic(q, AddrPair{local_port, ""})
	{
	}

	void open();

	void close();

	void sendTo(const AddrPair& remote, const MsgWrapper& msg);

	fd_t fd(){ return _fd; }
	
	void listenOnRecv(const OnRecvCb& cb){ _onrecv_cb=cb; }

private:
	void onInput();

private:
	fd_t _fd;
	AddrPair _local_addr;
	std::unique_ptr<InputEvent> _evt;
	InputListner _listener; 
	OnRecvCb _onrecv_cb;

};

}//net
NAMESP_END

#include "udppeer_basic.inl"

#endif /*UDPPEER_BASIC_H*/
