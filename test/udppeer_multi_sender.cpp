#include <udppeer.h>

#include <iostream>
#include <stdio.h>
#include <thread>
#include <atomic>
#include <vector>

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

	MySession(EventQueueEpoll* eq, const AddrPair& peer_addr, int local_port)
        :_peer_addr(peer_addr)
	{
		
		_udp = new UdpPeer4(eq, local_port);
		_udp->open();
		_udp->listenOnRecv([this](const AddrPair& addr, MsgSt m){this->onInput(addr, m);});
	}

	~MySession(){
		_udp->close();
	}

	void onInput(const AddrPair& addr, MsgSt msg) 
	{
		//cout<<"onInput["<<addr.ip<<":"<<addr.port<<"]"<<endl;
		//cout<<msg.begin()<<"("<<msg.size()<<")"<<endl;

        stat_now.to_packet_recv++;
	}

    void sendTo(const uint8_t* data, int size){
        MsgSt msg(data, size);
		_udp->sendTo(_peer_addr, msg);
    }

	UdpPeer4 *_udp;
    AddrPair _peer_addr;
};

using session_ptr = std::shared_ptr<MySession>;


int main(int argc, char** argv)
{
    if(argc < 4){
        std::cout<<"usage: cmd [remote ip] [num of ports] [port_base]"<<std::endl;
        return 0;
    }

	EventQueueEpoll eq;
    uint8_t buf[172]={
        0x80, 0x00, 0x0f, 0xa0, 0x00, 0x0d, 0xd8, 0x60, 0xca, 0x11, 0x00, 0x1f, 0x88, 0x7f, 0x74, 0x68,
	    0x5d, 0x61, 0x5b, 0x66, 0x86, 0x9e, 0xad, 0x9f, 0x81, 0x82, 0x87, 0x85, 0x95, 0x99, 0x94, 0x96,
	    0x91, 0x8a, 0x87, 0x83, 0x73, 0x5c, 0x51, 0x4a, 0x4c, 0x72, 0x90, 0xa2, 0xa4, 0x88, 0x6d, 0x65,
	    0x6b, 0x7e, 0x89, 0x89, 0x97, 0x9d, 0x98, 0x9a, 0x93, 0x81, 0x74, 0x6a, 0x59, 0x51, 0x71, 0x94,
	    0xa1, 0xa8, 0x92, 0x76, 0x6c, 0x6a, 0x72, 0x84, 0x87, 0x82, 0x81, 0x83, 0x8d, 0x8b, 0x7c, 0x78,
	    0x71, 0x64, 0x5a, 0x5f, 0x7a, 0x96, 0xa6, 0x96, 0x7b, 0x76, 0x73, 0x6f, 0x78, 0x81, 0x7f, 0x85,
	    0x88, 0x90, 0x9c, 0x96, 0x82, 0x6f, 0x64, 0x65, 0x60, 0x74, 0xa2, 0xbd, 0xbc, 0xab, 0x93, 0x7d,
	    0x78, 0x7b, 0x7b, 0x7a, 0x88, 0x90, 0x89, 0x89, 0x82, 0x6f, 0x61, 0x54, 0x55, 0x5f, 0x64, 0x80,
	    0x98, 0x9f, 0xa0, 0x8c, 0x76, 0x7b, 0x81, 0x85, 0x8e, 0x91, 0x90, 0x96, 0x9e, 0x96, 0x84, 0x81,
	    0x75, 0x5d, 0x59, 0x53, 0x5f, 0x86, 0x9a, 0xa3, 0x9f, 0x85, 0x73, 0x6b, 0x66, 0x75, 0x7d, 0x82,
	    0x93, 0xa2, 0x9f, 0x97, 0x88, 0x73, 0x64, 0x60, 0x55, 0x4e, 0x73, 0x95
    };
    int size=16;
    
    int nthreads=atoi(argv[2]);

    std::cout<<"num of threads:"<<nthreads<<std::endl;
    std::vector<std::thread> threads;
    std::vector<session_ptr> sessions;

    int port_base = atoi(argv[3]);

    for(int i=0; i<nthreads; i++){
        int port=port_base+i*2;
        int lport=port_base+i*2;
        AddrPair peer_addr={argv[1], port};
        session_ptr s( new MySession(&eq, peer_addr, lport));
        sessions.push_back(s);


        //sender
        threads.push_back(std::thread([s, buf, size](){
                    int delay=10;
                    while(true){
                        s->sendTo(buf, size);
                        stat_now.to_packet_send++;
                        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                    }
        }));

    }

    std::thread stst([size](){
            int tick_n=1000;
            int tick=tick_n;
            int delay=10;
            while(true){
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
                tick--;
                if(tick == 0){
                    int n = stat_now.to_packet_send - stat_pre.to_packet_send;
                    int packet_per_second = n/(tick_n*delay/1000);
                    std::cout<<"=================================================="<<std::endl;
                    std::cout<<"total packets sent:"<<stat_now.to_packet_send<<std::endl;
                    std::cout<<"packets sent/second:"<<packet_per_second<<std::endl;
                    std::cout<<"bitrate:"<<((float)packet_per_second*size*8)/1024.0<<" Kbs"<<std::endl;
                    tick=tick_n;
                    stat_pre.to_packet_send.store(stat_now.to_packet_send.load());
                    n = stat_now.to_packet_recv - stat_pre.to_packet_recv;
                    packet_per_second = n/(tick_n*delay/1000);
                    std::cout<<"=================================================="<<std::endl;
                    std::cout<<"total packets recv:"<<stat_now.to_packet_recv<<std::endl;
                    std::cout<<"packets recv/second:"<<packet_per_second<<std::endl;
                    std::cout<<"bitrate:"<<((float)packet_per_second*size*8)/1024.0<<" Kbs"<<std::endl;
                    std::cout<<"\n\n\n";
                    tick=tick_n;
                    stat_pre.to_packet_recv.store(stat_now.to_packet_recv.load());
                }
            }

        }
    );
	while(true)
	{
		eq.process();
	}

	getchar();

    return 0;
}

