//udp_test.cpp

#include "udppeer_co.h"
#include <iostream>

using namespace arsee::net;
using namespace std;

using UdpPeer = UdpPeer_co4;

struct CoTask{
  struct promise_type {
    CoTask get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};


CoTask loop(UdpPeer& udp)
{
    cout<<"enter loop coroutine\n";
    while(true){
       AddrPair remote;
       cout<<"call asnyc_read\n";
       auto msg = co_await udp.async_read(remote);
       cout<<"read from["<<remote.ip<<","<<remote.port<<"] size["<<msg.size()<<"]"<<endl;
       int w = co_await udp.async_write(remote, msg);
       cout<<"write to["<<remote.ip<<","<<remote.port<<"] size["<<msg.size()<<"]"<<endl;
    }
    cout<<"leave loop coroutine\n";
}

int main(int argc, char** argv)
{

    int port=10010;

    EventQueueEpoll eq;
    UdpPeer udp(&eq, port);

    
    loop(udp);
    //co_spawn(loop(udp));
    //CoExeScope scope;
    //co_spawn(scope, loop(udp));

    cout<<"after loop\n";
    while(true){
       eq.process();
    }

    return 0;
}
