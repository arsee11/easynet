//acceptor_completed_test.cpp

#include "../acceptor.h"
#include "../netlisteners.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;

struct MySession 
{

	MySession(NetEventQueue* eq)
		:_alistener(eq)
	{
		_alistener.listen([this](const netpeer_ptr& peer){ this->onAccept(peer); });
	}

	void onAccept(const netpeer_ptr& peer)
	{
		_peer = peer;
		cout<<"onAccept["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
	}

	AcceptCmpListener _alistener;
	netpeer_ptr _peer;
};

int main()
{
	NetEventQueue eq;
	AcceptorC a(&eq, 10000);	

	eq.attach( a.fd() );

	MySession s(&eq);

	while(true)
	{
		eq.process();
	}

}
