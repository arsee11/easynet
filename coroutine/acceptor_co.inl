//acceptor_co.inl

#include <coroutine>
#include <iostream>

NAMESP_BEGIN
namespace net
{

template<class Acceptor, class EventQueueT, class TcpPeer>
Acceptor_co<Acceptor, EventQueueT, TcpPeer>::Acceptor_co(
    EventQueueT* q, const AddrPair& local_addr
)
    :_acceptor(q, local_addr)
{
}

template<class Acceptor, class EventQueueT, class TcpPeer>
auto Acceptor_co<Acceptor, EventQueueT, TcpPeer>::async_accept()
{
    using AcceptorH = Acceptor_co<Acceptor, EventQueueT, TcpPeer>;
    struct Awaitable{
        Awaitable(AcceptorH* a):m_acceptor(a){
            std::cout<<__FUNCTION__<<"\n";
            m_acceptor->_acceptor.listenOnAccept([this](auto peer){
                if(m_awaiting_coro != nullptr){
                    m_peer = peer;
                    m_awaiting_coro.resume();
                }
            });
        }
        ~Awaitable(){
            std::cout<<__FUNCTION__<<"\n";
            m_acceptor->_acceptor.listenOnAccept(nullptr);
        }

        bool await_ready(){
            std::cout<<"await_ready\n";
            return false;
        }

        void await_suspend(std::coroutine_handle<> awaiting_coro){
            std::cout<<"await_suspend\n";
            m_awaiting_coro = awaiting_coro;
        }

        peer_ptr await_resume(){
            std::cout<<"await_resume\n";
            return peer_ptr(new TcpPeer(m_peer));
        }
        
        AcceptorH* m_acceptor;
        typename Acceptor::netpeer_ptr m_peer;
        std::coroutine_handle<> m_awaiting_coro;
    };

    return Awaitable(this);
}

}//net
NAMESP_END

