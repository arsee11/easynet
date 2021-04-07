//udppeer_test.cpp

#include "udppeer.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;

std::string ip;
struct MySession
{

	MySession(EventQueueEpoll* eq)
	{
		
		_udp.reset( new UdpPeer4(eq, AddrPair{ip, 10000}));
		_udp->open();
		_udp->listenOnRecv([this](const AddrPair& addr, const MsgSt& m){
				this->onInput(addr, m);});
		
	}

	~MySession(){
		_udp->close();
	}

	void onInput(const AddrPair& addr, const MsgSt& msg) 
	{
		cout<<"onInput["<<addr.ip<<":"<<addr.port<<"]"<<endl;
		cout<<msg.rd_ptr()<<"("<<msg.size()<<")"<<endl;
		_udp->sendTo(addr, msg);
	}

	std::unique_ptr<UdpPeer4> _udp;
};

int main(int argc, char** argv)
{
    ip = argv[1];
	EventQueueEpoll eq;
	MySession s(&eq);

	while(true)
	{
		eq.process();
	}

	getchar();

}
