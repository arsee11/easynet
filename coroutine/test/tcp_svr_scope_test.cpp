//tcp_svr_test.cpp

#include "acceptor_co.h"
#include "arseeulib/threading/co_exescope.h"
#include "arseeulib/threading/exescope.h"
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
    cout<<"call async_read thread:"<<std::this_thread::get_id()<<endl;
    auto msg = co_await peer->async_read();
    cout<<"read size["<<msg.size()<<"] thread:"<<std::this_thread::get_id()<<endl;
    int w = co_await peer->async_write(msg);
    cout<<"write size["<<msg.size()<<"] thread:"<<std::this_thread::get_id()<<endl;
}

using Scope = arsee::CoExeScope<arsee::ExeScope_p<EventQueueEpoll>>;

CoTask listen(Scope& scope, Acceptor& a)
{
    co_await scope.schedule();

    cout<<"enter listen coroutine thread:"<<std::this_thread::get_id()<<endl;
    while(true){
        cout<<"call async_accept\n";
        auto peer = co_await a.async_accept();
        if(!peer->is_valid() ){
            cout<<"peer is invalid\n"; 
            continue;
        }
        cout<<"accept peer["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port
            <<"] thread:"<<std::this_thread::get_id()<<endl;
        echo(peer);
    }
    cout<<"leave listen coroutine thread:"<<std::this_thread::get_id()<<endl;
}


int main(int argc, char** argv)
{
    int port=10010;

    EventQueueEpoll eq;
    Scope scope(&eq);

    Acceptor a(&eq, port);
    listen(scope, a);
    //co_spawn(scope, listen(tcp));

    cout<<"after listen\n";
    char ch;
    cin>>ch;

    return 0;
}
