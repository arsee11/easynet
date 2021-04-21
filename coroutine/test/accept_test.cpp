//accpet_test.cpp

#include "acceptor_co.h"
#include "event_queue_epoll.h"
#include "tcp_netpeer.h"
#include "socket.h"
#include <iostream>

using namespace arsee::net;
using namespace std;

using Acceptor =Acceptor_co4;

struct CoTask{
  struct promise_type {
    CoTask get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};


CoTask loop(Acceptor& acceptor)
{
    cout<<"enter loop coroutine\n";
    auto peer = co_await acceptor.async_accept();
    auto remote = peer->remote_addr();
    cout<<"accept ["<<remote.ip<<":"<<remote.port<<"]"<<endl;
    cout<<"leave loop coroutine\n";
}

int main(int argc, char** argv)
{
    int port=10010;

    EventQueueEpoll eq;
    Acceptor a(&eq, port);
    
    loop(a);
    //co_spawn(loop(udp));
    //CoExeScope scope;
    //co_spawn(scope, loop(udp));

    cout<<"after loop\n";
    while(true){
       eq.process();
    }

    return 0;
}
