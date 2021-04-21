//tcp_svr_test.cpp

#include "acceptor_co.h"
#include <iostream>

using namespace arsee::net;
using namespace std;
using Acceptor = Acceptor_co4;
using peer_ptr = Acceptor::peer_ptr;

struct CoTask{
  struct promise_type {
    CoTask get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};

CoTask echo(peer_ptr peer)
{
    cout<<"call async_read\n";
    auto msg = co_await peer->async_read();
    cout<<"read size["<<msg.size()<<"]"<<endl;
    int w = co_await peer->async_write(msg);
    cout<<"write size["<<msg.size()<<"]"<<endl;
}

CoTask listen(Acceptor& a)
{
    cout<<"enter listen coroutine\n";
    while(true){
        cout<<"call async_accept\n";
        auto peer = co_await a.async_accept();
        if(!peer->is_valid() ){
            cout<<"peer is invalid\n"; 
            continue;
        }
        cout<<"accept peer["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]\n";
        echo(peer);
    }
    cout<<"leave listen coroutine\n";
}


int main(int argc, char** argv)
{
    int port=10010;

    EventQueueEpoll eq;
    Acceptor a(&eq, port);

    listen(a);
    //co_spawn(listen(tcp));

    cout<<"after listen\n";
    while(true){
       eq.process();
    }

    return 0;
}
