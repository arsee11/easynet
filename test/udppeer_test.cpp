//udppeer_test.cpp

#include "../udppeer.h"
#include "../netlisteners.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;

struct MySession  
{

	MySession(NetEventQueue* eq)
		:_ilistener(eq)
	{
		
		_udp = new UdpPeer(eq, 10000);
		_udp->open();
		_ilistener.listen(_udp->fd(), [this](const AddrPair& addr, MsgSt m){this->onInput(addr, m);});
	}

	~MySession(){
		_ilistener.unlisten(_udp->fd());
		_udp->close();
	}

	void onInput(const AddrPair& addr, MsgSt msg) 
	{
		cout<<"onInput["<<addr.ip<<":"<<addr.port<<"]"<<endl;
		cout<<msg.begin()<<"("<<msg.size()<<")"<<endl;
		_udp->sendTo(addr, msg);
	}

	UdpInputListener _ilistener;
	UdpPeer *_udp;
};

int main()
{
	NetEventQueue eq;
	MySession s(&eq);

	while(true)
	{
		eq.process();
	}

	getchar();

}
