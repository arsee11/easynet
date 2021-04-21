///udppeer_co.inl

#include <iostream>

NAMESP_BEGIN
namespace net
{

template<class MsgWrapper>
void NetPeer_co<MsgWrapper>::onRecv(const AddrPair& remote, const MsgWrapper& msg)
{
    if(_in_msg_buf.size() >= _max_in_msg_size){
        std::cout<<"in_msg_buf overflow, size["<<_max_in_msg_size<<"]\n";
        return;
    }
    _in_msg_buf.push_back(Msg{msg, remote});
    auto coro = popAwaitingCoro();
    if(coro != nullptr){
        coro.resume();
    }
}

template<class MsgWrapper>
size_t NetPeer_co<MsgWrapper>::in_msg_size()
{
    return _in_msg_buf.size();
}    

template<class MsgWrapper>
auto NetPeer_co<MsgWrapper>::popInMsg()
{
    if(_in_msg_buf.size() > 0){
        auto msg= _in_msg_buf.front();
        _in_msg_buf.pop_front();
        return msg;
    }
    return Msg();
}

template<class MsgWrapper>
void NetPeer_co<MsgWrapper>::pushAwaitingCoro(
    std::coroutine_handle<> awaiting_coro
)
{
    _awaiting_coros.push_back(awaiting_coro);
}

template<class MsgWrapper>
void NetPeer_co<MsgWrapper>::clear()
{
    size_t n = _awaiting_coros.size();
    for(size_t i=0; i<n; i++){
        auto coro = _awaiting_coros.front();
        _awaiting_coros.pop_front();
        if(coro != nullptr){
            coro.destroy();
        }
    }
}

template<class MsgWrapper>
std::coroutine_handle<> NetPeer_co<MsgWrapper>::popAwaitingCoro()
{
    if(_awaiting_coros.size() > 0){
        auto coro = _awaiting_coros.front();
        _awaiting_coros.pop_front();
        return coro;
    }
    return nullptr;
}

}//net
NAMESP_END


