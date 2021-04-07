//udp_test.cpp


using namespace arsee::net;
using namespace std;

CoTask<void> loop(UdpPeer& udp)
{
    MsgSt msg(10240);
    while(true){
       int r = co_read_async(udp, msg);
       cout<<"read ["<<r<<"]"<<endl;
       int w = co_write_async(udp, msg);
       cout<<"write ["<<w<<"]"<<endl;
    }
}

int main(int argc, char** argv)
{

    int port=10010;

    NetEventQueue eq;
    coro::UdpPeer4 udp(eq, 10010);

    
    loop(udp);
    //co_spawn(loop(udp));
    //CoExeScope scope;
    //co_spawn(scope, loop(udp));

    while(true){
       eq.process();
    }

    return 0;
}
