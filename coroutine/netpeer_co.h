///udppeer_co.h

#ifndef	NETPEER_CO_H
#define NETPEER_CO_H

#include "fddef.h"
#include "addr.h"
#include "namespdef.h"
#include <coroutine>
#include <deque>

NAMESP_BEGIN
namespace net
{

template<class MsgWrapper>
class NetPeer_co 
{

public:
	NetPeer_co()
	{
	}

	virtual ~NetPeer_co(){
	}
    
protected:
    void clear();

    struct AwaitableWrite{
        bool await_ready(){
            return true;
        }
    
        void await_suspend(std::coroutine_handle<> awaiting_coro){
            m_awaiting_coro = awaiting_coro;
        }
    
        int await_resume(){
            return m_requested_msg.size();;
        }
        
        const MsgWrapper& m_requested_msg;
        std::coroutine_handle<> m_awaiting_coro;
    };
    
    using Peer = NetPeer_co<MsgWrapper>;
    struct AwaitableRecv{
        bool await_ready(){
            if(m_peer->in_msg_size() > 0){
                return true;
            }

            return false;
        }

        void await_suspend(std::coroutine_handle<> awaiting_coro){
            m_awaiting_coro = awaiting_coro;
            m_peer->pushAwaitingCoro(m_awaiting_coro);
        }

        MsgWrapper await_resume(){
            auto msg = m_peer->popInMsg();
            if(m_remote_addr != nullptr){
                *m_remote_addr = msg.addr;
            }
            return msg.msg;
        }
        
        Peer* m_peer=nullptr;
        AddrPair* m_remote_addr=nullptr;
        std::coroutine_handle<> m_awaiting_coro;
    };

    void onRecv(const AddrPair& remote, const MsgWrapper& msg);

private:
    size_t in_msg_size();
    auto popInMsg();
    void pushAwaitingCoro(std::coroutine_handle<> coro);
    std::coroutine_handle<> popAwaitingCoro();

private:
    struct Msg{
        MsgWrapper msg;
        AddrPair addr;
    };
    std::deque<Msg> _in_msg_buf;
    int _max_in_msg_size = 1024;
    std::deque<std::coroutine_handle<>> _awaiting_coros;

};

}//net
NAMESP_END

#include "netpeer_co.inl"

#endif /*NETPEER_CO_H*/
