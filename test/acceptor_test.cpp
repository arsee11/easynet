//acceptor_test.cpp

#include "../acceptor.h"
#include "../netlisteners.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace std;
using namespace std::placeholders;


struct MySession : public AcceptListener
{
	
	MySession(NetEventQueue* eq)
		:AcceptListener(eq)
	{
		this->listen( std::bind(&MySession::onAccept, this, _1) );
		//this->l();
	}
	
	void onAccept(fd_t fd)
	{
		sockaddr_in addr;
		socklen_t len = sizeof(addr);	
		fd_t newfd = ::accept(fd, (sockaddr*)&addr, &len);
		cout<<"onAccept("<<newfd<<"):"<<inet_ntoa(addr.sin_addr)<<" "<<ntohs(addr.sin_port)<<endl;
	}

};

int main()
{
	NetEventQueue eq;
	AcceptorR a(&eq, 10000);	
	AcceptorR a2(&eq, 10001);	
	eq.attach(a.fd());
	eq.attach(a2.fd());

	MySession s(&eq);

	while(true)
	{
		eq.process();
	}

	s.unlisten();
}
