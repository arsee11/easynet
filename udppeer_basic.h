///udppeer_basic.h

#ifndef	UDPPEER_BASIC_H 
#define UDPPEER_BASIC_H 

#include "fddef.h"
#include "addr.h"
#include "pollevents.h"
#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

template<class EventQueueT, class MsgWrapper>
class UdpPeerBasic 
{
	
	using PollInEvent = PollInputEvent<EventQueueT>;
	using PollListner = typename PollInEvent::listener_t;
	using InputEvent = UdpInputEvent<EventQueueT, MsgWrapper>;

public:
	UdpPeerBasic(EventQueueT* q, const AddrPair& local_addr)throw(sockexcpt)
		:_local_addr(local_addr)
		,_evt_queue(q)
		,_listener(q)
	{
	}

	UdpPeerBasic(EventQueueT* q, unsigned short local_port)throw(sockexcpt)
		:UdpPeerBasic(q, AddrPair{local_port, ""})
	{
	}

	void open()throw(sockexcpt);

	void close();

	void sendTo(const AddrPair& remote, const MsgWrapper& msg);

	fd_t fd(){ return _fd; }

	void onInput(fd_t fd);

private:
	fd_t _fd;
	AddrPair _local_addr;
	EventQueueT* _evt_queue;
	PollListner _listener; 

};

}//net
NAMESP_END

#include "udppeer_basic.inl"

#endif /*UDPPEER_BASIC_H*/
