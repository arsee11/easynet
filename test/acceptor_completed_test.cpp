//acceptor_completed_test.cpp

#include "acceptor.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;

struct MySession 
{

	void onAccept(netpeer_ptr peer)
	{
		_peer = peer;
		cout<<"onAccept["
			<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"
			<<endl;
	}

	netpeer_ptr _peer;
};

int main()
{
	EventQueueEpoll eq;
	AcceptorC a(&eq, 10000);	
	MySession s;
	a.listenOnAccept(std::bind(&MySession::onAccept, &s, _1));

	while(true)
	{
		eq.process();
	}

}
