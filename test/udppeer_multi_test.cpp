//udppeer_test.cpp

#include "../udppeer.h"
#include "../netlisteners.h"

#include <iostream>
#include <stdio.h>
#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <threading/exescope.h>

using arsee::ExeScope;

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;

struct Statistics
{
        std::atomic_int to_packet_send;
        std::atomic_int to_packet_recv;
};
Statistics stat_pre, stat_now;

struct MySession  
{

	MySession(NetEventQueue* eq, int local_port, ExeScope* st)
		:_ilistener(eq)
        ,send_thread(st)
	{
		
		_udp = new UdpPeer(eq, local_port);
		_udp->open();
		_ilistener.listen(_udp->fd(), [this](const AddrPair& addr, MsgSt m){this->onInput(addr, m);});
	}

	~MySession(){
		_ilistener.unlisten(_udp->fd());
		_udp->close();
	}

	void onInput(const AddrPair& addr, MsgSt msg) 
	{

        stat_now.to_packet_recv++;
		//cout<<"thread:"<<std::this_thread::get_id()<<" onInput["<<addr.ip<<":"<<addr.port<<"]"<<endl;
		//cout<<msg.begin()<<"("<<msg.size()<<")"<<endl;
        send_thread->post([this, addr, msg](){
		        _udp->sendTo(addr, msg);
            }    
        );
	}

	UdpInputListener _ilistener;
	UdpPeer *_udp;
    ExeScope* send_thread;
};

using session_ptr = std::shared_ptr<MySession>;

int main(int argc, char** argv)
{
    if(argc != 2){
        std::cout<<"usage: cmd [num of ports]"<<std::endl;
        return 0;
    }


    int num_of_eq=1;
    std::vector<NetEventQueue*> eqs;
    std::vector<std::thread> threads;
    for(int i=0; i<num_of_eq; i++){
        NetEventQueue* eq = new NetEventQueue;
        eqs.push_back(eq);
        threads.push_back(std::thread([eq](){
	            while(true)
	            {
	            	eq->process();
	            }
            })
        );

    }
    std::vector<ExeScope*> send_threads;
    send_threads.push_back( new ExeScope);
    //send_threads.push_back( new ExeScope);
    //send_threads.push_back( new ExeScope);
    //send_threads.push_back( new ExeScope);

    std::vector<session_ptr> sessions;
    int num=atoi(argv[1]);
    for(int i=0; i<num; i++){
        int nport=20000+i*2;
        NetEventQueue* eq = eqs[ i%num_of_eq ];
        session_ptr s( new MySession(eq, nport, send_threads[0]));
        sessions.push_back(s);
        session_ptr s2( new MySession(eq, nport+1, nullptr));
        sessions.push_back(s2);
    }

   
    std::thread stst([](){
            int tick_n=10;
            int tick=tick_n;
            int delay=1000;
            int size=16;
            while(true){
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                tick--;
                if(tick == 0){
                    int n = stat_now.to_packet_recv - stat_pre.to_packet_recv;
                    int packet_per_second = n/(tick_n*delay/1000);
                    std::cout<<"=================================================="<<std::endl;
                    std::cout<<"total packets recv:"<<stat_now.to_packet_recv<<std::endl;
                    std::cout<<"packets recv/second:"<<packet_per_second<<std::endl;
                    std::cout<<"bitrate:"<<((float)packet_per_second*size*8)/1024.0<<" Kbs"<<std::endl;
                    std::cout<<"\n";
                    tick=tick_n;
                    stat_pre.to_packet_recv.store(stat_now.to_packet_recv.load());
                }
            }

        }
    );

	getchar();

}
