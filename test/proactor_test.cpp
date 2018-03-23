//test.cpp

#include <iostream>
#include <strings.h>
#include <string.h>
#include <memory>
#include <string>
#include "../proactor.h"
#include "../epoll.h"
#include "../acceptor.h"
#include "../netevent_listener.h"

using namespace std;
using namespace arsee::net;

typedef Proactor<Epoll> proactor_t;

class MyNetEventListener: public NetworkEventListener
{
public: 
	void onNewPeer(NetPeer* conn)
	{
		cout<<"new peer{"<<conn->remote_addr().ip<<","<<conn->remote_addr().port<<"}"<<endl;	
	}	

	void onInput(const void* buf, size_t len, NetPeer* conn)
	{
		cout<<conn<<",recv{"<<conn->remote_addr().ip<<","<<conn->remote_addr().port<<"}";
		cout<<"("<<len<<"):"<<(const char*)buf<<endl;	
	}	

	void onOutput(NetPeer* conn)
	{
		
	}


	void onClose(NetPeer* conn)
	{
		cout<<"peer{"<<conn->remote_addr().ip<<","<<conn->remote_addr().port<<"} closed"<<endl;	
	}

};

int main( )
{
	Acceptor a(10000, false);
	a.open();

	MyNetEventListener l;
	l.listen<NetInputEvent>();
	l.listen<NetAcceptEvent>();
	l.listen<NetCloseEvent>();
	EventQueue eq;
	eq.addListener(&l);

	proactor_t p(&eq);
	p.setAcceptor(&a);

	while(true)
		p.run();

	return 0;
};
