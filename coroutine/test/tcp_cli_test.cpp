//tcp_cli_test.cpp

#include "connector_co.h"
#include <iostream>

using namespace arsee::net;
using namespace std;

struct CoTask{
  struct promise_type {
    CoTask get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};


CoTask loop(Connector_co4::peer_t& tcp)
{
    cout<<"enter loop coroutine\n";
    while(true){
       MsgSt msg("hello", 6);
       int w = co_await tcp.async_write(msg);
       cout<<"write size["<<msg.size()<<"]"<<endl;
       auto msgr = co_await tcp.async_read();
       cout<<"read size["<<msgr.size()<<"]"<<endl;
    }
    cout<<"leave loop coroutine\n";
}

int main(int argc, char** argv)
{

    int port=10010;

    EventQueueEpoll eq;
    Connector_co4 c(&eq, 0);
    auto peer = c.connect({"127.0.0.1", 10010});
    if(!peer.is_valid() ){
        return 1;
    }

    loop(peer);
    //co_spawn(loop(tcp));
    //CoExeScope scope;
    //co_spawn(scope, loop(tcp));

    cout<<"after loop\n";
    while(true){
       eq.process();
    }

    return 0;
}
