//udppeer_test.cpp

#include "../udppeer.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;

struct MySession
{

	MySession(EventQueueEpoll* eq)
	{
		
		_udp.reset( new UdpPeer(eq, 10000));
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
		cout<<msg.begin()<<"("<<msg.size()<<")"<<endl;
		_udp->sendTo(addr, msg);
	}

	std::unique_ptr<UdpPeer> _udp;
};

int main()
{
	EventQueueEpoll eq;
	MySession s(&eq);

	while(true)
	{
		eq.process();
	}

	getchar();

}
