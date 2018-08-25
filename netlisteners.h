//netlisteners.h

#ifndef	NET_LISTENERS_H 
#define NET_LISTENERS_H 

#include "netevent_queue.h"
#include "netevents.h"
#include "netpeer.h"
#include "msg.h"

#include <memory>

NAMESP_BEGIN
namespace net
{

using AcceptEvent = NetAcceptEvent<NetEventQueue>;

class AcceptListener : public AcceptEvent::listener_t
{
public:
	AcceptListener(NetEventQueue* q)
		:AcceptEvent::listener_t(q)
	{
	}

	void listen(const evthandler_t& h){
		AcceptEvent::listener_t::listen<AcceptEvent>(h);
	}

	void unlisten(){
		AcceptEvent::listener_t::unlisten<AcceptEvent>();
	}
};

using NetPeer = NetPeerBasic<NetEventQueue, MsgSt>;
using netpeer_ptr = std::shared_ptr<NetPeer>;

using AcceptCmpEvent= AcceptCompletedEvent<NetEventQueue, netpeer_ptr>;
class AcceptCmpListener : public AcceptCmpEvent::listener_t
{
public:
	AcceptCmpListener(NetEventQueue* q)
		:AcceptCmpEvent::listener_t(q)
	{
	}

	void listen(const evthandler_t& h){
		AcceptCmpEvent::listener_t::listen<AcceptCmpEvent>(h);
	}

	void unlisten(){
		AcceptCmpEvent::listener_t::unlisten<AcceptCmpEvent>();
	}
};


using CloseCmpEvent= CloseCompletedEvent<NetEventQueue, netpeer_ptr>;
class CloseCmpListener : public CloseCmpEvent::listener_t
{
public:
	CloseCmpListener(NetEventQueue* q)
		:CloseCmpEvent::listener_t(q)
	{
	}

	void listen(fd_t fd, const evthandler_t& h){
		CloseCmpEvent::listener_t::listen<CloseCmpEvent>(fd, h);
	}

	void unlisten(fd_t fd){
		AcceptCmpEvent::listener_t::unlisten<CloseCmpEvent>(fd);
	}
};

using InputCmpEvent= InputCompletedEvent<NetEventQueue, netpeer_ptr, MsgSt>;
class InputCmpListener : public InputCmpEvent::listener_t
{
public:
	InputCmpListener(NetEventQueue* q)
		:InputCmpEvent::listener_t(q)
	{
	}

	void listen(fd_t fd, const evthandler_t& h){
		InputCmpEvent::listener_t::listen<InputCmpEvent>(fd, h);
	}

	void unlisten(fd_t fd){
		InputCmpEvent::listener_t::unlisten<InputCmpEvent>(fd);
	}
};

using UdpInputE=UdpInputEvent<NetEventQueue, MsgSt>;
class UdpInputListener : public UdpInputE::listener_t
{
public:
	UdpInputListener(NetEventQueue* q)
		:UdpInputE::listener_t(q)
	{
	}

	void listen(fd_t fd, const evthandler_t& h){
		UdpInputE::listener_t::listen<UdpInputE>(fd, h);
	}

	void unlisten(fd_t fd){
		UdpInputE::listener_t::unlisten<UdpInputE>(fd);
	}
};
}//net
NAMESP_END

#endif /*NET_LISTENERS_H*/
