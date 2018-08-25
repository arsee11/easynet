//netevents.h

#ifndef NET_EVENTS_H
#define NET_EVENTS_H

#include "event.h"
#include "fddef.h"
#include "event_listener_basic.h"
#include "namespdef.h"

#include <tuple>
#include <functional>
#include <assert.h>

NAMESP_BEGIN
namespace net
{

///@param I index 1,2,3,...,n
///@param Func funtion with n args
///@param Tuple std::tuple instance
template<size_t... I, typename Func, typename Tuple>
void call_tuple_arg(const Func& f, const Tuple& t){
	f( std::get<I>(t)... );
}


template<class Listener, class... Params>
class NetEvent: public Event
{
public:
	using listener_t = Listener;

public:
	NetEvent(const listener_list& ls, Params... params)
		:_listeners(ls)
	{
		_tuple = std::make_tuple(params...);
	}

protected:

	template<int... I>
	void call(){
		for(auto& i : this->_listeners)
		{
			listener_t *l = static_cast<listener_t*>(i);
			call_tuple_arg<I...>(*l, this->_tuple);
		}
	}
	

protected:
	listener_list _listeners;
	std::tuple<Params...> _tuple;
};


using FdHandler = std::function<void(fd_t)>;

template<class EventQueueT>
using FdListener = EventListenerBasic<EventQueueT, FdHandler, fd_t>;  

template<class EventQueueT>
class NetInputEvent : public NetEvent<FdListener<EventQueueT>,fd_t>
{
	using Base = NetEvent<FdListener<EventQueueT>, fd_t>;

public:
	NetInputEvent(const listener_list& ls, fd_t fd)
		:Base(ls, fd)
	{
	}

	void fire()override{
		for(auto& i : this->_listeners)
		{
			this->template call<0>();
		}
	}
};


template<class EventQueueT>
class NetCloseEvent : public NetInputEvent<EventQueueT>
{
public:
	NetCloseEvent(const listener_list& ls, fd_t fd)
		:NetInputEvent<EventQueueT>(ls, fd)
	{
	}
};

template<class EventQueueT>
class NetAcceptEvent : public NetInputEvent<EventQueueT> 
{
public:
	NetAcceptEvent(const listener_list& ls, fd_t fd)
		:NetInputEvent<EventQueueT>(ls, fd)
	{
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
class InputCompletedEvent : public NetEvent<InputCompletedListener<
	EventQueueT, NetPeerPtr, MsgWrapper> , IN_C_PARAMS> 
{ 
public:
	using Base=NetEvent<InputCompletedListener<
		EventQueueT, NetPeerPtr, MsgWrapper>, IN_C_PARAMS>;

public: 
	InputCompletedEvent(const listener_list& ls, IN_C_PARAMS_N)
		:Base(ls, peer, msg)
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
class AcceptCompletedEvent: public NetEvent<AcceptCompletedListener<EventQueueT,NetPeerPtr> , A_C_PARAMS>
{
	using Base=NetEvent<AcceptCompletedListener<EventQueueT,NetPeerPtr>, A_C_PARAMS>;

public:
	AcceptCompletedEvent(const listener_list& ls, A_C_PARAMS_N)
		:Base(ls, peer)
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
	CloseCompletedEvent(const listener_list& ls, A_C_PARAMS_N)
		:AcceptCompletedEvent<EventQueueT, NetPeerPtr>(ls, peer)
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
class UdpInputEvent : public NetEvent<
	UdpInputListenerT<EventQueueT, MsgWrapper>, IN_U_PARAMS> 
{ 
public:
	using Base=NetEvent<
		UdpInputListenerT<EventQueueT, MsgWrapper>, IN_U_PARAMS>;

public: 
	UdpInputEvent(const listener_list& ls, IN_U_PARAMS_N)
		:Base(ls, peer, msg)
	{
	}

	void fire()override{ this->template call<0,1>(); }
};



////////////////////////////////////////////////////////////////////////////////
}//net
NAMESP_END

#endif /*NET_EVENTS_H*/
