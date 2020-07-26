//netevents.h

#ifndef NET_EVENTS_H
#define NET_EVENTS_H

#include "event.h"
#include "fddef.h"
#include "addr.h"
#include "event_listener_basic.h"
#include "namespdef.h"

#include <tuple>
#include <functional>
#include <assert.h>

NAMESP_BEGIN
namespace net
{

using NHandler = std::function<void()>;

template<class EventQueueT>
using NListener = EventListenerBasic<EventQueueT, NHandler>;  

template<class EventQueueT>
class NetInputEvent : public EventBasic<NListener<EventQueueT>>
{
public:
	NetInputEvent()
	{
	}

	void fire()override{
                this->template call();
	}
};


template<class EventQueueT>
class NetCloseEvent : public NetInputEvent<EventQueueT>
{
public:
	NetCloseEvent()
		:NetInputEvent<EventQueueT>()
	{
	}
};

template<class EventQueueT>
class NetAcceptEvent : public NetInputEvent<EventQueueT>
{
public:
	NetAcceptEvent()
	{
	}

	void fire()override{
                this->template call();
	}
};




////////////////////////////////////////////////////////////////////////////////
//

#define IN_C_PARAMS NetPeerPtr, MsgWrapper
#define IN_C_PARAMS_N const NetPeerPtr& peer, MsgWrapper msg

template<class NetPeerPtr, class MsgWrapper>
using InDataHandler= std::function<void(IN_C_PARAMS_N)>;

template<class EventQueueT, class NetPeerPtr, class MsgWrapper>
using InputCompletedListener = EventListenerBasic<EventQueueT
	,InDataHandler<NetPeerPtr, MsgWrapper>, IN_C_PARAMS>;

template<class EventQueueT, class NetPeerPtr, class MsgWrapper>
class InputCompletedEvent : public EventBasic<InputCompletedListener<
	EventQueueT, NetPeerPtr, MsgWrapper> , IN_C_PARAMS> 
{ 
public:
	using Base=EventBasic<InputCompletedListener<
		EventQueueT, NetPeerPtr, MsgWrapper>, IN_C_PARAMS>;

public: 
	InputCompletedEvent(IN_C_PARAMS_N)
		:Base( peer, msg)
	{
	}

	void fire()override{ this->template call<0,1>(); }
};



////////////////////////////////////////////////////////////////////////////////
//
#define A_C_PARAMS NetPeerPtr
#define A_C_PARAMS_N const NetPeerPtr& peer

template<class NetPeerPtr>
using PeerHandler= std::function<void(A_C_PARAMS_N)>;

template<class EventQueueT, class NetPeerPtr>
using AcceptCompletedListener = EventListenerBasic<EventQueueT, PeerHandler<NetPeerPtr>, A_C_PARAMS>;

template<class EventQueueT, class NetPeerPtr>
class AcceptCompletedEvent: public EventBasic<AcceptCompletedListener<EventQueueT,NetPeerPtr> , A_C_PARAMS>
{
	using Base=EventBasic<AcceptCompletedListener<EventQueueT,NetPeerPtr>, A_C_PARAMS>;

public:
	AcceptCompletedEvent(A_C_PARAMS_N)
		:Base( peer)
	{
	}

	void fire()override{ this->template call<0>(); }

};


////////////////////////////////////////////////////////////////////////////////
//
template<class EventQueueT, class NetPeerPtr>
class CloseCompletedEvent : public AcceptCompletedEvent<EventQueueT, NetPeerPtr>
{
public:
	CloseCompletedEvent(A_C_PARAMS_N)
		:AcceptCompletedEvent<EventQueueT, NetPeerPtr>( peer)
	{
	}
};


////////////////////////////////////////////////////////////////////////////////
//

#define IN_U_PARAMS AddrPair, MsgWrapper
#define IN_U_PARAMS_N const AddrPair& peer, MsgWrapper msg

template<class MsgWrapper>
using UInDataHandler= std::function<void(IN_U_PARAMS_N)>;

template<class EventQueueT, class MsgWrapper>
using UdpInputListenerT = EventListenerBasic<EventQueueT
	,UInDataHandler<MsgWrapper>, IN_U_PARAMS>;

template<class EventQueueT, class MsgWrapper>
class UdpInputEvent : public EventBasic<
	UdpInputListenerT<EventQueueT, MsgWrapper>, IN_U_PARAMS> 
{ 
public:
	using Base=EventBasic<
		UdpInputListenerT<EventQueueT, MsgWrapper>, IN_U_PARAMS>;

public: 
	UdpInputEvent(IN_U_PARAMS_N)
		:Base( peer, msg)
	{
	}

	void fire()override{ this->template call<0,1>(); }
};



////////////////////////////////////////////////////////////////////////////////
}//net
NAMESP_END

#endif /*NET_EVENTS_H*/
