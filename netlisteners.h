//netlisteners.h

#ifndef	NET_LISTENERS_H 
#define NET_LISTENERS_H 

#include "event_queue_epoll.h"
#include "netevents.h"
#include "netpeer.h"
#include "msg.h"

#include <memory>

NAMESP_BEGIN
namespace net
{

using AcceptEvent = NetAcceptEvent<EventQueueEpoll>;

using NetPeer = NetPeerBasic<EventQueueEpoll, MsgSt>;
using netpeer_ptr = std::shared_ptr<NetPeer>;

using AcceptCmpEvent= AcceptCompletedEvent<EventQueueEpoll, netpeer_ptr>;
class AcceptCmpListener : public AcceptCmpEvent::listener_t
{
public:
	AcceptCmpListener(EventQueueEpoll* q)
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


using CloseCmpEvent= CloseCompletedEvent<EventQueueEpoll, netpeer_ptr>;
class CloseCmpListener : public CloseCmpEvent::listener_t
{
public:
	CloseCmpListener(EventQueueEpoll* q)
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

using InputCmpEvent= InputCompletedEvent<EventQueueEpoll, netpeer_ptr, MsgSt>;
class InputCmpListener : public InputCmpEvent::listener_t
{
public:
	InputCmpListener(EventQueueEpoll* q)
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

using UdpInputE=UdpInputEvent<EventQueueEpoll, MsgSt>;
class UdpInputListener : public UdpInputE::listener_t
{
public:
	UdpInputListener(EventQueueEpoll* q)
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
